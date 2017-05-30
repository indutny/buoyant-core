#ifndef SRC_STACK_H_
#define SRC_STACK_H_

#include "assert.h"

typedef struct buoyant__stack_s buoyant__stack_t;

struct buoyant__stack_s {
  void** data;
  void** start;
  void** end;
};


static void buoyant__stack_push(buoyant__stack_t* s, void* data) {
  assert(s->data >= s->end);
  (*s->data--) = data;
}


static void* buoyant__stack_pop(buoyant__stack_t* s) {
  assert(s->data < s->start);
  return (*++s->data);
}


static void buoyant__stack_reserve(buoyant__stack_t* s, unsigned int num) {
  s->data -= num;
  assert(s->end < s->data && s->data <= s->start);
}


static void buoyant__stack_restore(buoyant__stack_t* s, unsigned int num) {
  s->data += num;
  assert(s->end < s->data && s->data <= s->start);
}

#endif  /* SRC_STACK_H_ */
