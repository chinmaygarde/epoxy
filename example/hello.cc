// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

// This file is generated and present in the gen/ build outputs directory.
#include "hello.h"

#include <iostream>

namespace hello {

void SayHello() {
  std::cout << "Hello from native code!" << std::endl;
}

double AddNumbers(int32_t a, int64_t b, float c) {
  return a + b + c;
}

int32_t* GetIntPointer() {
  static int32_t some_int = 123;
  return &some_int;
}

double* GetDoublePointer() {
  static double some_double = 456.0;
  return &some_double;
}

void AddPointerValues(int32_t* a, double* b) {
  const auto value = *a + *b;
  std::cout << "The values added up are " << value << std::endl;
}

Hello* HelloCreate() {
  auto hello = new Hello();
  hello->val = 42;
  std::cout << "Create Hello." << std::endl;
  return hello;
}

void HelloDestroy(Hello* object) {
  std::cout << "Destroy Hello." << std::endl;
  delete object;
}

Goodbye* GoodbyeCreate() {
  return new Goodbye();
}

void GoodbyeDestroy(Goodbye* object) {
  delete object;
}

void TakeEnumValue(HelloEnum val) {}

}  // namespace hello
