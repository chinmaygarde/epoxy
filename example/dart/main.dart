import 'dart:ffi' as ffi;

import 'lib/bindings.dart';

main() {
  final dylib = ffi.DynamicLibrary.open("example/hello/libhello.dylib");
  SaySomethingFunc hello_func = dylib.lookup<ffi.NativeFunction<SaySomethingFuncC>>("SaySomething").asFunction();
  hello_func();

  SumFunc sum_func = dylib.lookup<ffi.NativeFunction<SumFuncC>>("Sum").asFunction();
  print(sum_func(10, 20));
}
