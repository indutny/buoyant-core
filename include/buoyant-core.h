#ifndef INCLUDE_BUOYANT_CORE_H_
#define INCLUDE_BUOYANT_CORE_H_

#include <stdint.h>

#define BUOYANT_IOP(CODE, A, B, C) \
    buoyant_build_opcode(kBuoyantInternalOpcode##CODE, (A), (B), (C))
#define BUOYANT_WIDE_IOP(CODE, A, W) \
    buoyant_build_wide_opcode(kBuoyantInternalOpcode##CODE, (A), (W))

/* Forward declaration */
typedef struct buoyant_s buoyant_t;
typedef struct buoyant_opcode_handler_s buoyant_opcode_handler_t;

typedef uint32_t buoyant_opcode_t;
typedef uint32_t buoyant_internal_opcode_t;
typedef uint8_t buoyant_opcode_id_t;
typedef uint8_t buoyant_arg_t;
typedef uint16_t buoyant_wide_arg_t;

typedef void (*buoyant_runtime_fn_t)(buoyant_t* b,
                                     buoyant_internal_opcode_t opcode);

/* `W` postfix means immediate wide argument */
enum buoyant_internal_opcode_id_e {
  /* return 0, 0 */
  kBuoyantInternalOpcodeReturn = 0,

  /* vmenter reg, 0 */
  kBuoyantInternalOpcodeVMEnter = 1,

  /* vmleave reg, 0 */
  kBuoyantInternalOpcodeVMLeave = 2,

  /* runtime reg, reg, id */
  kBuoyantInternalOpcodeRuntime = 3,

  /* `arg reg, index`
   * Index values:
   *   - 0 for A
   *   - 1 for B
   *   - 2 for C
   *   - 3 for W
   */
  kBuoyantInternalOpcodeArg = 4,
  kBuoyantInternalOpcodeAdd32 = 5,

  kBuoyantInternalOpcodeMax = 255,
  kBuoyantInternalOpcodeCount
};
typedef enum buoyant_internal_opcode_id_e buoyant_internal_opcode_id_t;

enum buoyant_default_opcode_id_e {
  /* enter 0, slots */
  kBuoyantDefaultOpcodeEnter = 0,
  /* leave 0, slots */
  kBuoyantDefaultOpcodeLeave = 1,

  /* Just an offset, not instruction */
  kBuoyantOpcodeStart,

  kBuoyantOpcodeMax = 255,
  kBuoyantOpcodeCount
};
typedef enum buoyant_default_opcode_id_e buoyant_default_opcode_id_t;

struct buoyant_opcode_handler_s {
  buoyant_internal_opcode_t* code;
  unsigned int len;
};

buoyant_t* buoyant_create(buoyant_runtime_fn_t runtime);
void buoyant_destroy(buoyant_t* b);

int buoyant_install_opcode(buoyant_t* b,
                           const buoyant_opcode_handler_t* handler,
                           buoyant_opcode_id_t* res);

void buoyant_run(buoyant_t* b, buoyant_opcode_t* code);

/* Helpers */
static buoyant_opcode_t buoyant_build_opcode(buoyant_opcode_id_t op_id,
                                             buoyant_arg_t A, buoyant_arg_t B,
                                             buoyant_arg_t C) {
  return op_id | (A << 8) | (B << 16) | (C << 24);
}


static buoyant_opcode_t buoyant_build_wide_opcode(buoyant_opcode_id_t op_id,
                                                  buoyant_arg_t A,
                                                  buoyant_wide_arg_t W) {
  return op_id | (A << 8) | (W << 16);
}


static buoyant_opcode_id_t buoyant_opcode_id(buoyant_opcode_t opcode) {
  return opcode & 0xff;
}


static buoyant_arg_t buoyant_opcode_a(buoyant_opcode_t opcode) {
  return (opcode >> 8) & 0xff;
}


static buoyant_arg_t buoyant_opcode_b(buoyant_opcode_t opcode) {
  return (opcode >> 16) & 0xff;
}


static buoyant_arg_t buoyant_opcode_c(buoyant_opcode_t opcode) {
  return (opcode >> 24) & 0xff;
}


static buoyant_wide_arg_t buoyant_opcode_w(buoyant_opcode_t opcode) {
  return opcode >> 16;
}

#endif  /* INCLUDE_BUOYANT_CORE_H_ */
