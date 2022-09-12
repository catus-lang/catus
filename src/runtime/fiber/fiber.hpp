#pragma once

#include "pch.hpp"

#include "memory/collector/collector.hpp"

namespace catus::runtime::fiber {
template <typename ...Args>
class Fiber {
public:
  Fiber(std::function<void(Fiber* self, Args...)> entrypoint);
  ~Fiber();

public:
  void invoke(Args... args) const;
  void yield(); // TODO: implement
  void recover(); // TODO: implement

private:
  uintptr_t context_; // TODO: use Coroutine class
  std::function<void(Fiber* self, Args...)> entrypoint_;
  memory::AbstractCollector* collector_;

private:
  bool exited_ = false;
};
} // namespace catus::runtime::fiber
