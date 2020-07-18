import 'dart:ffi' as ffi;

import '../../build/clang/example/gen/hello.dart';

main() {
  AttachNativeBindings();
  SayHello();
  print(AddNumbers(12, 14, 10.0));
  AddPointerValues(GetIntPointer(), GetDoublePointer());
  ffi.Pointer<Hello> hello = HelloCreate();
  print(hello.ref.val);
  HelloDestroy(hello);
}
