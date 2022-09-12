#pragma once

#include "pch.hpp"

#include "memory/collector/collector.hpp"

namespace catus::runtime::fiber {
class Fiber {
public:
  Fiber(uintptr_t entrypoint) {};
  ~Fiber() {};

public:
  void execute() {};

private:
  uintptr_t context;
  memory::CollectorGenGC collector;
};
} // namespace catus::runtime::fiber
