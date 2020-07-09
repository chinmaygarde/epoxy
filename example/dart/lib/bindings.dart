import 'dart:ffi' as ffi;

typedef SaySomethingFuncC = ffi.Void Function();
typedef SaySomethingFunc = void Function();

typedef SumFuncC = ffi.Int32 Function(ffi.Int32, ffi.Int32);
typedef SumFunc = int Function(int, int);
