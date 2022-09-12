#pragma once

#include "pch.hpp"

#include "abstract_collector.hpp"

#include "memory/managed_pointer/managed_pointer_gen_gc.hpp"

namespace catus::runtime::memory {
class CollectorGenGC : AbstractCollector {
public:
  CollectorGenGC();
  void release() override;

public:
  ManagedPointerGenGC* allocate(size_t size) override;
  void collect_auto() override;
  void collect(bool full) override;

private:
  ManagedPointerGenGC* allocate_gen0(size_t size);
  ManagedPointerGenGC* allocate_gen1(size_t size);
  void expand(size_t target);

private:
  const size_t kAlignedSize = 32; // 256-bit aligned
  const size_t kAlignedPage = 65536; // 64KB aligned
  const size_t kLargeObjectSize = 65536; // 64KB
  const size_t kMarkLongAliveCounter = 512; 
  const std::chrono::seconds kAutoCollectTimerThreshold = std::chrono::seconds(60);
  const uint64_t kAutoCollectCounterThreshold = 4;

private:
  void* gen_zero_pool_ = nullptr;
  size_t gen_zero_pool_size_ = 0;
  size_t gen_zero_pooL_offset_ = 0;

private:
  std::chrono::time_point<std::chrono::system_clock> last_collect_time_;
  uint64_t auto_collect_counter_ = 0;

private:
  ManagedPointerGenGC object_root_{0};
  std::unordered_set<ManagedPointerGenGC*> allocated_gen_zero_pointers_; // New and small objects
  std::unordered_set<ManagedPointerGenGC*> allocated_gen_one_pointers_; // Large or long-lived objects
};
} // namespace catus::runtime::memory
