import 'dart:ffi' as ffi;

// The bindings are generated at this spot.
import '../../build/clang/example/gen/dart/hello.dart';

main() {
  BindAll();
  SaySomething();
  print(Sum(10, 65));
}
