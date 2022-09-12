#pragma once

#include "pch.hpp"

#include "memory/collector/collector.hpp"

namespace catus::runtime::fiber {
class Fiber {
public:
  Fiber(std::function<int32_t(const Fiber* self, uint64_t argc, void* argv)> entrypoint);
  ~Fiber();

public:
  void invoke(uint64_t argc, void* argv);
  void yield(); // TODO: implement
  void recover(); // TODO: implement

private:
  uintptr_t context_; // TODO: use Coroutine class
  std::function<int32_t(const Fiber* self, uint64_t argc, void* argv)> entrypoint_;
  memory::AbstractCollector* collector_;

private:
  uintptr_t ideal_thread_ = 0; // use Thread class
  bool ideal_thread_fixed_ = false;

private:
  bool exited_ = false;
  int32_t exit_code_ = 0;
};
} // namespace catus::runtime::fiber
