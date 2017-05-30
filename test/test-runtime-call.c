#include "test/common.h"

static void runtime_cb(buoyant_t* b, buoyant_internal_opcode_t opcode) {
}


TEST_IMPL(runtime_call) {
  buoyant_t* b;
  buoyant_opcode_t code[2];

  b = buoyant_create(runtime_cb);
  CHECK_NE(b, NULL, "buoyant_create() failure");

  code[0] = buoyant_build_opcode(kBuoyantDefaultOpcodeRuntime, 1, 2, 3);
  code[1] = buoyant_build_wide_opcode(kBuoyantDefaultOpcodeLeave, 0, 4);

  buoyant_run(b, code);

  buoyant_destroy(b);
}
