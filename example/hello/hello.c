#include "hello.h"

#include <stdio.h>

void SaySomething() {
  printf("Hello\n");
}

int Sum(int a, int b) {
  return a + b;
}

void GiveHello(struct Hello* hello, int32_t var) {
  hello->some_value = var;
}
