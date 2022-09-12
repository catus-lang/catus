#include "fiber.hpp"

using namespace catus::runtime::fiber;

Fiber::Fiber(std::function<int32_t(const Fiber* self, uint64_t argc, void* argv)> entrypoint) : entrypoint_(entrypoint) {
  collector_ = new memory::CollectorGenGC();
}

Fiber::~Fiber() {
  delete collector_;
}

void Fiber::invoke(uint64_t argc, void* argv) {
  exit_code_ = entrypoint_(this, argc, argv);
  exited_ = true;
}
