#pragma once

#include "pch.hpp"

namespace catus::runtime::memory {
class AbstractManagedPointer {
public:
  ~AbstractManagedPointer() { release(); };
  virtual void release();

public:
  virtual volatile void* get() = 0;
  virtual size_t get_size() = 0;
};
} // namespace catus::runtime::memory
