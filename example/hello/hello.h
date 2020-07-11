#pragma once

#include <stdint.h>

__attribute__((visibility("default"))) void SaySomething(void);

__attribute__((visibility("default"))) int Sum(int a, int b);

struct Hello {
  int32_t some_value;
};

__attribute__((visibility("default"))) void GiveHello(struct Hello* hello,
                                                      int32_t var);
