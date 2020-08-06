# This source file is part of Epoxy licensed under the MIT License.
# See LICENSE.md file for details.

cmake_minimum_required(VERSION 3.0)

project(epoxy_example)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")

include(epoxy)

add_library(hello SHARED
  hello.cc
)

epoxy(hello
  cxx_interface.template.epoxy
  hello.epoxy
  hello.h
)

epoxy(hello
  cxx_impl.template.epoxy
  hello.epoxy
  hello_impl.cc
)

epoxy(hello
  dart.template.epoxy
  hello.epoxy
  hello.dart
)
