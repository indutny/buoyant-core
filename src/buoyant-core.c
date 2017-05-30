#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "include/buoyant-core.h"

#include "src/internal.h"

/* TODO(indutny): make this configurable */
#define DEFAULT_STACK_SIZE 16384

static void buoyant__add_default_opcodes(buoyant_t* b);


buoyant_t* buoyant_create(buoyant_runtime_fn_t runtime) {
  buoyant_t* res;

  res = calloc(1, sizeof(*res));
  if (res == NULL)
    return NULL;

  res->stack.data = calloc(DEFAULT_STACK_SIZE, sizeof(*res->stack.data));
  if (res->stack.data == NULL) {
    free(res);
    return NULL;
  }

  res->stack.start = res->stack.data;
  res->stack.end = &res->stack.data[DEFAULT_STACK_SIZE];

  res->pc = NULL;
  res->mode = kBuoyantDispatchNormal;

  res->runtime = runtime;

  buoyant__add_default_opcodes(res);

  return res;
}


void buoyant__add_default_opcodes(buoyant_t* b) {
  static buoyant_opcode_t code_enter[1];
  static buoyant_opcode_t code_leave[1];
  static buoyant_opcode_t code_runtime[2];

  code_enter[0] = BUOYANT_IOP(Enter, 0, 0, 0);
  code_leave[0] = BUOYANT_IOP(Leave, 0, 0, 0);

  b->opcode.handler[kBuoyantDefaultOpcodeEnter].code = code_enter;
  b->opcode.handler[kBuoyantDefaultOpcodeLeave].code = code_leave;

  code_runtime[0] = BUOYANT_IOP(Runtime, 0, 0, 0);
  code_runtime[1] = BUOYANT_WIDE_IOP(Leave, 0, 0);
  b->opcode.handler[kBuoyantDefaultOpcodeRuntime].code = code_runtime;

  b->opcode.count = kBuoyantOpcodeStart;
}


void buoyant_destroy(buoyant_t* b) {
  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(b->opcode.handler); i++) {
    free(b->opcode.handler[i].code);
    b->opcode.handler[i].code = NULL;
    b->opcode.handler[i].len = 0;
  }

  free(b);
}


int buoyant_register_opcode(buoyant_t* b,
                            const buoyant_opcode_handler_t* handler,
                            buoyant_opcode_id_t* res) {
  buoyant_opcode_id_t id;
  buoyant_internal_opcode_t* code;

  code = malloc(handler->len);
  if (code == NULL)
    return -1;

  memcpy(code, handler->code, handler->len);

  id = b->opcode.count++;
  b->opcode.handler[id].code = code;
  b->opcode.handler[id].len = handler->len;
  *res = id;

  return 0;
}


static void buoyant__stack_push(buoyant_t* b, void* data) {
  assert(b->stack.data < b->stack.end);
  (*b->stack.data++) = data;
}


static void* buoyant__stack_pop(buoyant_t* b) {
  assert(b->stack.data > b->stack.start);
  return (*--b->stack.data);
}


static void buoyant__stack_reserve(buoyant_t* b, void** value) {
  assert(b->stack.start <= value && value < b->stack.end);
}


static void buoyant__stack_restore(buoyant_t* b, void** value) {
  assert(b->stack.start <= value && value < b->stack.end);
}


static void buoyant__call(buoyant_t* b, buoyant_opcode_t* code,
                          buoyant_dispatch_mode_t mode) {
  buoyant__stack_push(b, b->pc);
  buoyant__stack_push(b, (void*) (intptr_t) b->mode);
  b->pc = code;
  b->mode = mode;
}


static void buoyant__return(buoyant_t* b) {
  b->mode = (intptr_t) buoyant__stack_pop(b);
  b->pc = buoyant__stack_pop(b);
}


void buoyant_run(buoyant_t* b, buoyant_opcode_t* code) {
  void** stack;

  stack = b->stack.data;
  buoyant__call(b, code, kBuoyantDispatchNormal);

  while (b->stack.data != stack) {
    buoyant_opcode_t opcode;
    buoyant_opcode_id_t opcode_id;
    const buoyant_opcode_handler_t* handler;

    opcode = *(b->pc++);
    opcode_id = buoyant_opcode_id(opcode);

    if (b->mode == kBuoyantDispatchNormal) {
      handler = &b->opcode.handler[opcode_id];
      assert(handler->code != NULL);

      buoyant__call(b, handler->code, kBuoyantDispatchInternal);
      continue;
    }

    switch (opcode_id) {
      case kBuoyantInternalOpcodeEnter:
        /* TODO(indutny): implement me */
        break;
      case kBuoyantInternalOpcodeLeave:
        /* TODO(indutny): implement me */
        buoyant__return(b);
        break;
      case kBuoyantInternalOpcodeRuntime:
        b->runtime(b, opcode);
        break;
      default:
        abort();
    }
  }
}
