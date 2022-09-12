#include "collector_no_gc.hpp"

using namespace catus::runtime::memory;

void CollectorNoGC::release() {
  for (ManagedPointerNoGC* pointer: allocated_pointers_) {
    delete pointer;
  }
}

ManagedPointerNoGC* CollectorNoGC::allocate(size_t size) {
  ManagedPointerNoGC* pointer = new ManagedPointerNoGC(size);
  allocated_pointers_.push_back(pointer);
  return pointer;
}
