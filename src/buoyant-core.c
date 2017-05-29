#include "include/buoyant-core.h"

#include "src/internal.h"

buoyant_core_t* buoyant_create(buoyant_runtime_fn_t runtime) {
  return 0;
}


void buoyant_destroy(buoyant_core_t* b) {
}


buoyant_opcode_id_t buoyant_register_opcode(
      buoyant_core_t* b, const buoyant_internal_opcode_t* code,
      unsigned int code_len) {
  return 0;
}


int buoyant_run(buoyant_core_t* b, const buoyant_opcode_t* code,
                unsigned int code_len) {
  return 0;
}
