#pragma once

#include "pch.hpp"

#include "fiber.hpp"

namespace catus::runtime::fiber {
class Thread {
public:
  Thread() {};
  void dispatch(Fiber* fiber, uint64_t argc, void* argv);
  void dispatch(Fiber* fiber);

private:
  void run();

private:
  std::atomic<bool> busy_ { false };
};
}
