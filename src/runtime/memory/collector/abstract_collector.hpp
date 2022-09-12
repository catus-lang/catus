#pragma once

#include "pch.hpp"

#include "memory/managed_pointer/abstract_managed_pointer.hpp"

namespace catus::runtime::memory {
class AbstractCollector {
public:
  ~AbstractCollector() { release(); };
  virtual void release();

public:
  virtual AbstractManagedPointer* allocate(size_t size);
  virtual void collect_auto() = 0;
  virtual void collect(bool full) = 0;
};
} // namespace catus::runtime::memory
