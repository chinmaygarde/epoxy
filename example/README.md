# Epoxy Example

This example demonstrates how to use CMake to generate C++ and Dart interfaces when the associated `.epoxy` file is updated.

The generated files are not meant to be checked into version control and are created and referenced from the build output directory.

If you are using CMake, you should be set as you can just copy the [tools/](tools/) directory into your CMake modules and use the `epoxy` macro in your CMake definitions. Integrating into other build systems should be very similar.

## Overview

The significant pieces of this example:
* `hello.epoxy`: Contains the interface from which C++ and Dart bindings will be generated.
* `cxx_interface.template.epoxy`: This is the Inja template used to generate the C++ header that you will use to provide you native implementation to Dart.
* `cxx_impl.template.epoxy`: This is the Inja template that will be used to generate the native C entrypoints from Dart. This layer desugars stuff like namespaces and enums and makes sure C++ name mangling does not get in the way of making your native implementation visible to Dart.
* `dart.template.epoxy`: Contains the Inja template that generates Dart bindings. This layer also handles sugaring of enums and setting up bindings upfront.

## Recommendation

Copy the `.template.epoxy` files into your own projects and use them as the base in your projects. This example is an un-opinionated take on C++ and Dart bindings but you have the flexibility to edit these to suit your own projects. For example, you may want to integrate the `structs` into a custom object system.
