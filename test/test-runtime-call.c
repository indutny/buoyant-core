#include "test/common.h"

static unsigned int called;

static void runtime_cb(buoyant_t* b, buoyant_arg_t id, void* f, void* s) {
  CHECK_EQ(called, 0, "must be called no more than once");
  CHECK_EQ(id, 13, "runtime id");
  CHECK_EQ((intptr_t) f, 23, "runtime first");
  CHECK_EQ((intptr_t) s, 42, "runtime second");
  called++;
}


TEST_IMPL(runtime_call) {
  buoyant_t* b;
  buoyant_internal_opcode_t opcode_impl[4];
  buoyant_opcode_handler_t opcode_handler;
  buoyant_opcode_id_t opcode;
  buoyant_opcode_t code[3];

  b = buoyant_create(runtime_cb);
  CHECK_NE(b, NULL, "buoyant_create() failure");

  opcode_impl[0] = BUOYANT_WIDE_IOP(Arg, 0, 0);
  opcode_impl[1] = BUOYANT_WIDE_IOP(Arg, 1, 3);
  opcode_impl[2] = BUOYANT_IOP(Runtime, 13, 0, 1);
  opcode_impl[3] = BUOYANT_WIDE_IOP(Return, 0, 0);

  opcode_handler.code = opcode_impl;
  opcode_handler.len = sizeof(opcode_impl);

  CHECK_EQ(buoyant_install_opcode(b, &opcode_handler, &opcode), 0,
           "register_opcode() failure");

  code[0] = buoyant_build_wide_opcode(kBuoyantDefaultOpcodeEnter, 0, 4);
  code[1] = buoyant_build_wide_opcode(opcode, 23, 42);
  code[2] = buoyant_build_wide_opcode(kBuoyantDefaultOpcodeLeave, 0, 4);

  buoyant_run(b, code);

  buoyant_destroy(b);

  CHECK_EQ(called, 1, "must be called once");
}
