// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

// This file is generated and present in the gen/ build outputs directory.
#include "hello.h"

#include <cstring>
#include <iostream>

namespace hello {

void SayHello() {
  std::cout << "Hello from C++ code." << std::endl;
}

int32_t* GetIntPointer() {
  static int32_t value = 42;
  return &value;
}

int32_t AddValues(int32_t* a, int32_t b) {
  return *a + b;
}

Hello* CreateHello(HelloType type) {
  std::cout << "Creating a hello struct." << std::endl;
  auto hello = new Hello();
  std::memset(hello, 0, sizeof(Hello));
  hello->type = type;
  return hello;
}

void DestroyHello(Hello* hello) {
  std::cout << "Destroying a hello struct." << std::endl;
  delete hello;
}

HelloType TakeHelloType() {
  return HelloType::LongWinded;
}

void DoSomethingElse() {}

}  // namespace hello
