#include "fiber.hpp"

using namespace catus::runtime::fiber;

template <typename ...Args>
Fiber<Args...>::Fiber(std::function<void(Fiber* self, Args...)> entrypoint) : entrypoint_(entrypoint) {
  collector_ = new memory::CollectorGenGC();
}

template <typename ...Args>
Fiber<Args...>::~Fiber() {
  delete collector_;
}

template <typename ...Args>
void Fiber<Args...>::invoke(Args... args) const {
  entrypoint_(this, args...);
  exited_ = true;
}
