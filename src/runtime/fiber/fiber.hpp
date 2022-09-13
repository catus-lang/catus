#pragma once

#include "pch.hpp"

#include "memory/collector/collector.hpp"

namespace catus::runtime::fiber {
class Thread;

class Fiber {
public:
  Fiber(std::function<int32_t(const Fiber* self, uint64_t argc, void* argv)> entrypoint);
  ~Fiber();

public:
  void invoke(uint64_t argc, void* argv);
  void yield(); // TODO: implement
  void recover(); // TODO: implement

public:
  void set_ideal_thread(Thread* thread);

private:
  uintptr_t context_; // TODO: use Coroutine class
  std::function<int32_t(const Fiber* self, uint64_t argc, void* argv)> entrypoint_;
  memory::AbstractCollector* collector_;

private:
  Thread* ideal_thread_ = nullptr;

private:
  bool exited_ = false;
  int32_t exit_code_ = 0;
};
} // namespace catus::runtime::fiber
