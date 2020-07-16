import 'dart:ffi' as ffi;

import 'lib/bindings.dart';

main() {
  BindAll();
  SaySomething();
  print(Sum(10, 65));
}
