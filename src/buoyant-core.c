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

  res->vm_stack.data = calloc(DEFAULT_STACK_SIZE, sizeof(*res->vm_stack.data));
  if (res->vm_stack.data == NULL) {
    free(res);
    return NULL;
  }

  res->vm_stack.start = res->vm_stack.data;
  res->vm_stack.end = &res->vm_stack.data[DEFAULT_STACK_SIZE];

  res->pc = NULL;
  res->mode = kBuoyantDispatchNormal;

  res->runtime = runtime;

  buoyant__add_default_opcodes(res);

  return res;
}


void buoyant__add_default_opcodes(buoyant_t* b) {
  static buoyant_opcode_t code_enter[2];
  static buoyant_opcode_t code_leave[2];

  code_enter[0] = BUOYANT_IOP(VMEnter, 0, 0, 0);
  code_enter[1] = BUOYANT_WIDE_IOP(Return, 0, 0);
  b->opcode.handler[kBuoyantDefaultOpcodeEnter].code = code_enter;

  code_leave[0] = BUOYANT_IOP(VMLeave, 0, 0, 0);
  code_leave[1] = BUOYANT_WIDE_IOP(Return, 0, 0);
  b->opcode.handler[kBuoyantDefaultOpcodeLeave].code = code_leave;

  b->opcode.count = kBuoyantOpcodeStart;
}


void buoyant_destroy(buoyant_t* b) {
  unsigned int i;

  for (i = kBuoyantOpcodeStart; i < ARRAY_SIZE(b->opcode.handler); i++) {
    free(b->opcode.handler[i].code);
    b->opcode.handler[i].code = NULL;
    b->opcode.handler[i].len = 0;
  }

  free(b);
}


int buoyant_install_opcode(buoyant_t* b,
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


static void buoyant__call(buoyant_t* b, buoyant_opcode_t* code,
                          buoyant_dispatch_mode_t mode) {
  assert(b->mode == kBuoyantDispatchNormal);
  b->mode = mode;

  if (b->mode == kBuoyantDispatchInternal) {
    b->emulator.caller = b->pc;
  } else {
    buoyant__stack_push(&b->vm_stack, b->pc);
  }
  b->pc = code;
}


static void buoyant__return(buoyant_t* b) {
  if (b->mode == kBuoyantDispatchInternal) {
    b->pc = b->emulator.caller;
    b->emulator.caller = NULL;
    b->mode = kBuoyantDispatchNormal;
  } else {
    b->pc = buoyant__stack_pop(&b->vm_stack);
  }
}


void buoyant_run(buoyant_t* b, buoyant_opcode_t* code) {
  void** stack;

  stack = b->vm_stack.data;
  buoyant__call(b, code, kBuoyantDispatchNormal);

  while (b->vm_stack.data != stack) {
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
      case kBuoyantInternalOpcodeVMEnter:
        /* TODO(indutny): implement me */
        break;
      case kBuoyantInternalOpcodeVMLeave:
        /* TODO(indutny): implement me */
        buoyant__return(b);
        buoyant__return(b);
        break;
      case kBuoyantInternalOpcodeReturn:
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
