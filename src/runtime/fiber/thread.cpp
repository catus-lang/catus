#include "thread.hpp"

#include "fiber.hpp"

using namespace catus::runtime::fiber;

void Thread::dispatch(Fiber* fiber, uint64_t argc, void* argv) {
  busy_ = true;
  fiber->set_ideal_thread(this);
  fiber->invoke(argc, argv);
  busy_ = false;
}

void Thread::dispatch(Fiber* fiber) {
  busy_ = true;
  fiber->set_ideal_thread(this);
  fiber->recover();
  busy_ = false;
}
