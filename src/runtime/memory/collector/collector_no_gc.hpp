#pragma once

#include "pch.hpp"

#include "abstract_collector.hpp"

#include "memory/managed_pointer/managed_pointer_no_gc.hpp"

namespace catus::runtime::memory {
class CollectorNoGC : AbstractCollector{
public:
  CollectorNoGC() {};
  void release() override;

public:
  ManagedPointerNoGC* allocate(size_t size) override;
  void collect_auto() override {};
  void collect(bool full) override {};

private:
  std::vector<ManagedPointerNoGC*> allocated_pointers_;
};
} // namespace catus::runtime::memory
