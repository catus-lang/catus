cmake_minimum_required(VERSION 3.1)

project(catus_runtime)

set(CATUS_RUNTIME_SOURCE_FILES
  fiber/fiber.hpp
  fiber/scheduler.hpp
  fiber/thread.hpp
  memory/collector/abstract_collector.hpp
  memory/collector/collector.hpp
  memory/collector/collector_gen_gc.hpp
  memory/collector/collector_no_gc.hpp
  memory/managed_pointer/abstract_managed_pointer.hpp
  memory/managed_pointer/managed_pointer.hpp
  memory/managed_pointer/managed_pointer_gen_gc.hpp
  memory/managed_pointer/managed_pointer_no_gc.hpp
  pch.hpp
  exports.cpp
  fiber/fiber.cpp
  fiber/scheduler.cpp
  memory/collector/collector_gen_gc.cpp
  memory/collector/collector_no_gc.cpp
)

add_library(catus_runtime STATIC
  "${CATUS_RUNTIME_SOURCE_FILES}"
)

target_include_directories(catus_runtime PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)
