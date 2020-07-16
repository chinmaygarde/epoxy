// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

// This file is generated and present in the gen/ build outputs directory.
#include "hello.h"

#include <iostream>

namespace hello {

void SayHello() {
  std::cout << "Hello" << std::endl;
}

void SayHelloWithNumber(int32_t num) {
  std::cout << "Hello  " << num << std::endl;
}

int64_t AddNumbers(int32_t a, int64_t b) {
  return a + b;
}

void* GivePointerPlease() {
  return nullptr;
}

double Multiply2(double a, double b) {
  return a * b;
}

Hello* HelloCreate() {
  return new Hello();
}

void HelloDestroy(Hello* hello) {
  delete hello;
}

}  // namespace hello
