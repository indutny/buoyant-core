#include <stdlib.h>

#include "src/emulator.h"


static void buoyant__emulator_dispatch_vmenter(buoyant__emulator_t* e,
                                               buoyant__stack_t* stack,
                                               buoyant_opcode_t opcode) {
  buoyant_arg_t a;

  a = buoyant_opcode_a(opcode);

  buoyant__stack_reserve(stack, (intptr_t) e->regs[a]);
}


static void buoyant__emulator_dispatch_vmleave(buoyant__emulator_t* e,
                                               buoyant__stack_t* stack,
                                               buoyant_opcode_t opcode) {
  buoyant_arg_t a;

  a = buoyant_opcode_a(opcode);

  buoyant__stack_restore(stack, (intptr_t) e->regs[a]);
  e->caller.pc = buoyant__stack_pop(stack);
}


static void buoyant__emulator_dispatch_arg(buoyant__emulator_t* e,
                                           buoyant__stack_t* stack,
                                           buoyant_opcode_t opcode) {
  buoyant_arg_t a;
  void** reg;

  a = buoyant_opcode_a(opcode);
  reg = &e->regs[a];

  switch (buoyant_opcode_w(opcode)) {
    case 0:
      *reg = (void*) (intptr_t) buoyant_opcode_a(e->caller.opcode);
      break;
    case 1:
      *reg = (void*) (intptr_t) buoyant_opcode_b(e->caller.opcode);
      break;
    case 2:
      *reg = (void*) (intptr_t) buoyant_opcode_c(e->caller.opcode);
      break;
    case 3:
      *reg = (void*) (intptr_t) buoyant_opcode_w(e->caller.opcode);
      break;
    default:
      *reg = 0;
      break;
  }
}


static void buoyant__emulator_dispatch_runtime(buoyant__emulator_t* e,
                                               buoyant__stack_t* stack,
                                               buoyant_opcode_t opcode) {
  buoyant_arg_t id;
  void* first;
  void* second;

  id = buoyant_opcode_a(opcode);
  first = e->regs[buoyant_opcode_b(opcode)];
  second = e->regs[buoyant_opcode_c(opcode)];
  e->runtime(e->runtime_arg, id, first, second);
}


int buoyant__emulator_dispatch(buoyant__emulator_t* e, buoyant__stack_t* stack,
                               buoyant_opcode_t opcode) {
  switch (buoyant_opcode_id(opcode)) {
    case kBuoyantInternalOpcodeVMEnter:
      buoyant__emulator_dispatch_vmenter(e, stack, opcode);
      break;
    case kBuoyantInternalOpcodeVMLeave:
      buoyant__emulator_dispatch_vmleave(e, stack, opcode);
      break;
    case kBuoyantInternalOpcodeReturn:
      return 1;
    case kBuoyantInternalOpcodeArg:
      buoyant__emulator_dispatch_arg(e, stack, opcode);
      break;
    case kBuoyantInternalOpcodeRuntime:
      buoyant__emulator_dispatch_runtime(e, stack, opcode);
      break;
    default:
      abort();
  }

  return 0;
}
