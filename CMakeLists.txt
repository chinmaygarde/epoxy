cmake_minimum_required(VERSION 3.0)

project(epoxy)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")

include(platform_selection)
include(grammar)

detect_platform()

add_subdirectory(source)
add_subdirectory(example/hello)
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
