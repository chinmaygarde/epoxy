import 'dart:ffi' as ffi;

typedef SaySomethingFuncCT = ffi.Void Function();
typedef SaySomethingFuncT = void Function();

typedef SumFuncCT = ffi.Int32 Function(ffi.Int32, ffi.Int32);
typedef SumFuncT = int Function(int, int);

class Hello extends ffi.Struct {
  @ffi.Int32()
  int some_value;
}

typedef GiveHelloCT = ffi.Void Function(ffi.Pointer<Hello> hello, ffi.Int32 value);
typedef GiveHelloT = void Function(ffi.Pointer<Hello> hello, int value);

SaySomethingFuncT SaySomething;
SumFuncT Sum;
GiveHelloT GiveHello;

void BindAll() {
  final dylib = ffi.DynamicLibrary.open("example/hello/libhello.dylib");

  SaySomething = dylib.lookup<ffi.NativeFunction<SaySomethingFuncCT>>("SaySomething").asFunction();
  Sum = dylib.lookup<ffi.NativeFunction<SumFuncCT>>("Sum").asFunction();
  GiveHello = dylib.lookup<ffi.NativeFunction<GiveHelloCT>>("GiveHello").asFunction();
}
