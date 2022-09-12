#pragma once

#include "pch.hpp"

#include "abstract_managed_pointer.hpp"

namespace catus::runtime::memory {
class ManagedPointerNoGC : public AbstractManagedPointer {
public:
  ManagedPointerNoGC(size_t size) {
    size_ = size;
    pointer_ = malloc(size);
  };

  void release() override {
    free(pointer_);
  };

public:
  volatile void* get() override { return pointer_; }
  size_t get_size() override { return size_; }

private:
  void* pointer_ = nullptr;
  size_t size_ = 0;
};
} // namespace catus::runtime::memory
