#ifndef SRC_EMULATOR_H_
#define SRC_EMULATOR_H_

#include "include/buoyant-core.h"

#include "src/stack.h"

#define EMULATOR_REGS 256

typedef struct buoyant__emulator_s buoyant__emulator_t;

struct buoyant__emulator_s {
  buoyant_runtime_fn_t runtime;
  void* runtime_arg;

  struct {
    buoyant_opcode_t opcode;
    buoyant_opcode_t* pc;
  } caller;
  void* regs[EMULATOR_REGS];
};

int buoyant__emulator_dispatch(buoyant__emulator_t* e, buoyant__stack_t* stack,
                               buoyant_opcode_t opcode);

#endif  /* SRC_EMULATOR_H_ */
