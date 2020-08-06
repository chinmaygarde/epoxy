// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

import 'dart:ffi' as ffi;

import '../../build/clang/example/gen/hello.dart';

main() {
  // This must be done before using any of the generated utilities.
  AttachNativeBindings();

  SayHello();

  print(AddValues(GetIntPointer(), 99));

  var hello = CreateHello(HelloType.LongWinded);
  if (hello.ref.type == HelloType.LongWinded
   && hello.ref.type == TakeHelloType()) {
    print("Enums work idiomatically in Dart and C++.");
  }

  DestroyHello(hello);
}
