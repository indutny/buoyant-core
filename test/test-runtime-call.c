#include "test/common.h"

static void runtime_cb(buoyant_t* b, buoyant_internal_opcode_t opcode) {
}


TEST_IMPL(runtime_call) {
  buoyant_t* b;
  buoyant_internal_opcode_t opcode_impl[2];
  buoyant_opcode_handler_t opcode_handler;
  buoyant_opcode_id_t opcode;
  buoyant_opcode_t code[3];

  b = buoyant_create(runtime_cb);
  CHECK_NE(b, NULL, "buoyant_create() failure");

  opcode_impl[0] = BUOYANT_IOP(Runtime, 1, 2, 3);
  opcode_impl[1] = BUOYANT_WIDE_IOP(Return, 0, 0);

  opcode_handler.code = opcode_impl;
  opcode_handler.len = sizeof(opcode_impl);

  CHECK_EQ(buoyant_register_opcode(b, &opcode_handler, &opcode), 0,
           "register_opcode() failure");

  code[0] = buoyant_build_wide_opcode(kBuoyantDefaultOpcodeEnter, 0, 4);
  code[1] = buoyant_build_opcode(0, 1, 2, 3);
  code[2] = buoyant_build_wide_opcode(kBuoyantDefaultOpcodeLeave, 0, 4);

  buoyant_run(b, code);

  buoyant_destroy(b);
}
