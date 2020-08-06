# Epoxy

[![Linux](https://github.com/chinmaygarde/epoxy/workflows/Linux/badge.svg)](https://github.com/chinmaygarde/epoxy/actions?query=workflow%3ALinux)
[![Mac](https://github.com/chinmaygarde/epoxy/workflows/Mac/badge.svg)](https://github.com/chinmaygarde/epoxy/actions?query=workflow%3AMac)
[![Windows](https://github.com/chinmaygarde/epoxy/workflows/Windows/badge.svg)](https://github.com/chinmaygarde/epoxy/actions?query=workflow%3AWindows)

Epoxy is an IDL and Code Generator for Dart FFI Bindings.

![Epoxy Demo](example/demo.gif)

# CLI Reference

```
Epoxy FFI Bindings Code Generator
=================================

Usage:
------

    epoxy  --output <output file path>
           --idl    <Epoxy IDL file path>
           [--template-file <Template File Path>]
           [--template-default-cxx]
           [--template-default-dart]
           [--template-default-cxx-dump]
           [--template-default-dart-dump]
           [--template-data-dump]
           [--help]
           [--version]

Options:
--------

  --output            The path to the file containing the Code generated for the
                      given options. Optional if if --template-data-dump is
                      specified.

  --idl               The path the Epoxy IDL file.

  --template-file     The path to a custom code generation template. To
                      introspect the data used to render the template, use the
                      --template-data-dump option. The Inja template rendering
                      system is used to render the template data.

  --template-data-dump
                      Instead of rendering the code generation template, dump
                      the template data. This is useful when writing or
                      customizing a custom code generation template.

  --help              Dump these help instructions.

  --version           Get the Epoxy version.
```

# IDL Manual

The Epoxy interface definition can be contained in any file but is typically present in a file with the `.epoxy` suffix.

## Namespaces

Each Epoxy interface definition file must contain one or more namespaces. Namespaces are used to organize the structs and function within the interface definition file.

```
namespace hello {

} // namespace foo
```

## Primitives

The following primitives may be specified directly in an Epoxy definition file.

* `int8_t`: A signed 8-bit integer.
* `int16_t`: A signed 16-bit integer.
* `int32_t`: A signed 32-bit integer.
* `int64_t`: A signed 64-bit integer.
* `uint8_t`: An unsigned 8-bit integer.
* `uint16_t`: An unsigned 16-bit integer.
* `uint32_t`: An unsigned 32-bit integer.
* `uint64_t`: An unsigned 64-bit integer.
* `double`: A 64-bit double.
* `float`: A 32-bit float.

* `void`: The void type may be used to specify a pointer to an opaque type.

## Pointers

Pointers to primitive values or structs may be specified by prepending them with `*`.

```
function GiveIntPointer() -> int8_t*
function CreateHello() -> Hello*
function DestroyHello(Hello*) -> void
```

## Enums

Specify Enums just like you would in C/C++. Across a namespace, the enum or struct names may not be repeated.

```
enum MyEnum {
  Tinker,
  Tailor,
  Soldier,
}
```

## Structs

Within a namespace, there may be one or more `struct` definitions. Across a namespace, the enum or struct names may not be repeated.

```
struct Hello {
  int8_t some_int;
  double some_double;
  int64_t *pointer_to_some_64_bit_int;
}
```

## Functions

Zero or more functions may appear anywhere in a `namespace` definition. . Across a namespace, function names may not be repeated.

```
function AddTwoNumbers(int64_t a, int64_t b) -> int64_t
// void returns are implicit.
function ReturnVoid()
```


# Build Requirements

This project can be compiled on Windows, Linux, or, Mac. On the host, the follow dependencies are required. The `Dockerfile` provided in the project root may also be used to construct an image capable of building this project.

* A C++ 17 Compiler.
* [Flex 2.6.3 or higher](https://github.com/westes/flex).
* [Bison 3.3.2 or higher](https://www.gnu.org/software/bison/).
