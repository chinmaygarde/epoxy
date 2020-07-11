import 'dart:ffi' as ffi;

import 'lib/bindings.dart';

main() {
  BindAll();
  SaySomething();
  print(Sum(10, 65));
  Hello hello;
  Hello.allocate();
  GiveHello(hello.addressOf, 32);
  print(hello.some_value);
}
