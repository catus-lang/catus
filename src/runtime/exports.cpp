#include "pch.hpp"

#include "fiber/fiber.hpp"

extern "C" {
  void catus_runtime_initialize() {
    catus::runtime::fiber::Fiber<>(
      [](auto self) {
        std::cout << "Hello, world!" << std::endl;
    }).invoke();
  }
}
