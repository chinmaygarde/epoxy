# This source file is part of Epoxy licensed under the MIT License.
# See LICENSE.md file for details.

cmake_minimum_required(VERSION 3.0)

project(epoxy)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(
    $<$<CONFIG:>:/MT>
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

enable_testing()

set(EPOXY_FLEX_SEARCH_PATH  "" CACHE STRING "Path to the flex (>=2.6.3) program.")
set(EPOXY_BISON_SEARCH_PATH "" CACHE STRING "Path to the Bison (>=3.3.2) program.")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")

include(platform_selection)
include(grammar)

detect_platform()

add_subdirectory(source)
add_subdirectory(third_party/json EXCLUDE_FROM_ALL)
add_subdirectory(third_party/inja EXCLUDE_FROM_ALL)

set(EPOXY_BUILD_EXAMPLES YES CACHE BOOL "Build Examples")
if(EPOXY_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

set(EPOXY_BUILD_TESTS YES CACHE BOOL "Build Tests")
if(EPOXY_BUILD_TESTS)
  add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
endif()
