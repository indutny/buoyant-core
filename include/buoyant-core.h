#ifndef INCLUDE_BUOYANT_CORE_H_
#define INCLUDE_BUOYANT_CORE_H_

#include <stdint.h>

/* Forward declaration */
typedef struct buoyant_core_s buoyant_core_t;

typedef uint32_t buoyant_opcode_t;
typedef uint32_t buoyant_internal_opcode_t;
typedef uint8_t buoyant_opcode_id_t;
typedef uint8_t buoyant_arg_t;
typedef uint16_t buoyant_wide_arg_t;

typedef void (*buoyant_runtime_fn_t)(buoyant_core_t* b,
                                     buoyant_internal_opcode_t opcode);

enum buoyant_internal_opcode_id_e {
  kBuoyantInternalRuntime = 0,
  kBuoyantInternalAdd32 = 1
};
typedef enum buoyant_internal_opcode_id_e buoyant_internal_opcode_id_t;

buoyant_core_t* buoyant_create(buoyant_runtime_fn_t runtime);
void buoyant_destroy(buoyant_core_t* b);

buoyant_opcode_id_t buoyant_register_opcode(
      buoyant_core_t* b, const buoyant_internal_opcode_t* code,
      unsigned int code_len);

int buoyant_run(buoyant_core_t* b, const buoyant_opcode_t* code,
                unsigned int code_len);

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
