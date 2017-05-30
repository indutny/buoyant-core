#ifndef SRC_INTERNAL_H_
#define SRC_INTERNAL_H_

#include "include/buoyant-core.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

enum buoyant_dispatch_mode_e {
  kBuoyantDispatchNormal,
  kBuoyantDispatchInternal
};
typedef enum buoyant_dispatch_mode_e buoyant_dispatch_mode_t;

struct buoyant_s {
  buoyant_runtime_fn_t runtime;

  struct {
    buoyant_opcode_handler_t handler[kBuoyantOpcodeCount];
    unsigned int count;
  } opcode;

  struct {
    void** data;
    void** start;
    void** end;
  } stack;

  buoyant_opcode_t* pc;
  buoyant_dispatch_mode_t mode;
};

#endif  /* SRC_INTERNAL_H_ */
