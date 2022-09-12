#include "pch.hpp"

#include "fiber/fiber.hpp"

extern "C" {
  void catus_runtime_initialize() {
    catus::runtime::fiber::Fiber(
      [](auto self, uint64_t argc, void* argv) -> int32_t {
        std::cout << "Hello, world!" << std::endl;
        return 0;
    }).invoke(0, nullptr);
  }
}
