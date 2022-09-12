#include "collector_gen_gc.hpp"

using namespace catus::runtime::memory;

CollectorGenGC::CollectorGenGC() {
  // initialize the Gen 0 pool
  gen_zero_pool_ = malloc(kAlignedPage);
  gen_zero_pool_size_ = kAlignedPage;
  gen_zero_pooL_offset_ = 0;

  object_root_.set_generation(1); // Long lived by default
}

ManagedPointerGenGC* CollectorGenGC::allocate(size_t size) {
  collect_auto();

  if (size > kLargeObjectSize) {
    auto pointer = new ManagedPointerGenGC(size);
    pointer->set_pointer(malloc(size));
    pointer->set_generation(1);
    allocated_gen_one_pointers_.insert(pointer);
    return pointer;
  }

  size_t new_offset = (gen_zero_pooL_offset_ + size + kAlignedSize - 1) / kAlignedSize * kAlignedSize;

  if (new_offset < gen_zero_pool_size_) {
    expand(new_offset);
    return allocate(size);
  }

  auto pointer = new ManagedPointerGenGC(size);
  pointer->set_pointer((void*)((size_t)gen_zero_pool_ + gen_zero_pooL_offset_));
  pointer->set_generation(0);
  allocated_gen_zero_pointers_.insert(pointer);
  gen_zero_pooL_offset_ = new_offset;
  return pointer;
}

void CollectorGenGC::release() {
  for (auto pointer: allocated_gen_zero_pointers_) {
    delete pointer;
  }
  free(gen_zero_pool_);

  for (auto pointer: allocated_gen_one_pointers_) {
    if (pointer->get() != nullptr) {
      free((void*)pointer->get());
      pointer->set_pointer(nullptr);
    }
    delete pointer;
  }
}

void CollectorGenGC::collect_auto() {
  if (auto_collect_counter_ >= kAutoCollectCounterThreshold) {
    collect(false);
    return;
  }

  if (std::chrono::system_clock::now() - last_collect_time_ >= kAutoCollectTimerThreshold) {
    collect(false);
    return;
  }
}

void CollectorGenGC::collect(bool full) {
  last_collect_time_ = std::chrono::system_clock::now();
  auto_collect_counter_ = 0;

  // Mark phase, mark all the objects that are alive
  std::queue<ManagedPointerGenGC*> queue;
  std::unordered_set<ManagedPointerGenGC*> marked; // Alive objects
  size_t marked_gen0_size = 0;
  queue.push(&object_root_);
  while (!queue.empty()) {
    ManagedPointerGenGC* pointer = queue.front();
    queue.pop();
    if (marked.find(pointer) != marked.end()) {
      // Already marked
      continue;
    }

    marked.insert(pointer);
    pointer->add_alive_counter();

    if (pointer->get_generation() == 0) {
      if (pointer->get_alive_counter() >= kMarkLongAliveCounter) {
        // Move to Gen 1
        pointer->set_generation(1);
        void* ptr =  malloc(pointer->get_size());
        memcpy(ptr, (void*)pointer->get(), pointer->get_size());
        pointer->set_pointer(ptr);
        allocated_gen_one_pointers_.insert(pointer);
        allocated_gen_zero_pointers_.erase(pointer);
      } else {
        marked_gen0_size += (pointer->get_size() + kAlignedSize - 1) / kAlignedSize * kAlignedSize;
      }
    }
    
    for (ManagedPointerGenGC* child: pointer->get_children()) {
      queue.push(child);
    }
  }

  // Gen 0 Copy phase, copy all the objects that are alive
  size_t new_pool_size = (marked_gen0_size + kAlignedPage - 1) / kAlignedPage * kAlignedPage;
  void* new_pool = malloc(new_pool_size);
  size_t new_offset = 0;
  for (ManagedPointerGenGC* pointer: marked) {
    if (pointer->get_generation() == 0) {
      size_t size = (pointer->get_size() + kAlignedSize - 1) / kAlignedSize * kAlignedSize;
      void* new_pointer = (void*)((size_t)new_pool + new_offset);
      memcpy(new_pointer, (void*)pointer->get(), size);
      pointer->set_pointer(new_pointer);
      new_offset += size;
    }
  }
  free(gen_zero_pool_);
  gen_zero_pool_ = new_pool;
  gen_zero_pool_size_ = new_pool_size;
  gen_zero_pooL_offset_ = new_offset;

  if (!full) { return; }

  // Sweep phase, sweep all the objects that are dead
  std::vector<ManagedPointerGenGC*> gen_zero_pointers_to_delete;
  std::vector<ManagedPointerGenGC*> gen_one_pointers_to_delete;
  for (ManagedPointerGenGC* pointer: allocated_gen_zero_pointers_) {
    if (marked.find(pointer) == marked.end()) {
      gen_zero_pointers_to_delete.push_back(pointer);
    }
  }

  for (ManagedPointerGenGC* pointer: gen_zero_pointers_to_delete) {
    allocated_gen_zero_pointers_.erase(pointer);
    delete pointer;
  }

  for (ManagedPointerGenGC* pointer: allocated_gen_one_pointers_) {
    if (marked.find(pointer) == marked.end()) {
      gen_one_pointers_to_delete.push_back(pointer);
    }
  }

  for (ManagedPointerGenGC* pointer: gen_one_pointers_to_delete) {
    allocated_gen_one_pointers_.erase(pointer);
    free((void*)pointer->get());
    delete pointer;
  }
}

void CollectorGenGC::expand(size_t target) {
  if (target < gen_zero_pool_size_) {
    // Do nothing
    return;
  }

  size_t new_size = (target + kAlignedPage - 1) / kAlignedPage * kAlignedPage;
  void* new_pool = malloc(new_size);
  memcpy(new_pool, gen_zero_pool_, gen_zero_pool_size_);
  free(gen_zero_pool_);
  gen_zero_pool_ = new_pool;
  gen_zero_pool_size_ = new_size;

  for (ManagedPointerGenGC* pointer: allocated_gen_zero_pointers_) {
    pointer->set_pointer((void*)((size_t)gen_zero_pool_ + (size_t)pointer->get()));
  }
}
