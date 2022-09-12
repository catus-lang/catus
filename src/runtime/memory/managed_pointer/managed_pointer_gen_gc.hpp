#pragma once

#include "pch.hpp"

#include "abstract_managed_pointer.hpp"

namespace catus::runtime::memory {
class ManagedPointerGenGC : public AbstractManagedPointer {
public:
  ManagedPointerGenGC(size_t size) { size_ = size; };
  void release() override;

public:
  volatile void* get() noexcept override { return pointer_; };
  size_t get_size() noexcept override { return size_; };

public:
  void set_pointer(void* pointer) noexcept { pointer_ = pointer; }
  std::unordered_multiset<ManagedPointerGenGC*>& get_children() noexcept { return children_; }
  void set_generation(uint8_t generation) noexcept { generation_ = generation; }
  uint8_t get_generation() const noexcept { return generation_; }

public:
  void add_reference(ManagedPointerGenGC* pointer) noexcept { children_.insert(pointer); }
  void remove_reference(ManagedPointerGenGC* pointer) noexcept { children_.erase(pointer); }
  void add_alive_counter() noexcept { alive_counter_++; }
  uint64_t get_alive_counter() noexcept { return alive_counter_; }

private:
  volatile void* pointer_ = nullptr;
  size_t size_ = 0;
  uint8_t generation_ = 0;
  uint64_t alive_counter_ = 0;
  std::unordered_multiset<ManagedPointerGenGC*> children_;
};
} // namespace catus::runtime::memory
