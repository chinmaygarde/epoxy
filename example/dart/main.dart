import '../../build/clang/example/gen/hello.dart';

main() {
  AttachNativeBindings();
  SayHello();
  print(AddNumbers(12, 14, 10.0));
  AddPointerValues(GetIntPointer(), GetDoublePointer());

  var hello = HelloCreate();
  print(hello.ref.val);
}
