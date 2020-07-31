// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include <gtest/gtest.h>

#include "driver.h"
#include "file.h"
#include "fixture.h"

#include <sstream>
#include <string>

namespace epoxy {
namespace testing {

TEST(DriverTest, CanParseEmpty) {
  Driver driver;
  auto result = driver.Parse(R"~()~");
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
}

TEST(DriverTest, FailsOnSimpleInvalid) {
  Driver driver;
  auto result = driver.Parse(R"~(AASH*)A)~");
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
}

TEST(DriverTest, CanParseEmptyNamespaces) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {

    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetName(), "foo");
}

TEST(DriverTest, CanParseMultipleNamespaces) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {

    }

    namespace bar {

    }

    namespace baz {

    }

    namespace bang {

    }


    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 4u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetName(), "foo");
  ASSERT_EQ(driver.GetNamespaces()[1].GetName(), "bar");
  ASSERT_EQ(driver.GetNamespaces()[2].GetName(), "baz");
  ASSERT_EQ(driver.GetNamespaces()[3].GetName(), "bang");
}

TEST(DriverTest, CommentsAreIgnored) {
  Driver driver;
  auto result = driver.Parse(R"~(
    // This
    namespace foo {
      // Is a comment
    }
    // Another comment
    ////////// Another comment
    //// Another comment
    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetName(), "foo");
}

template <class T>
bool CheckFunctionReturnType(const Function::ReturnType& type, T value) {
  auto val = std::get_if<T>(&type);
  if (val == nullptr) {
    return false;
  }
  return *val == value;
}

TEST(DriverTest, CanParseFunctions) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      function hello(int64_t quick, int32_t brown, int64_t fox) -> int64_t
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetName(), "hello");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetArguments().size(),
            3);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[0]
                .GetIdentifier(),
            "quick");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[1]
                .GetIdentifier(),
            "brown");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[2]
                .GetIdentifier(),
            "fox");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[0]
                .GetPrimitive(),
            Primitive::kInt64);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[1]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[0]
                .GetArguments()[2]
                .GetPrimitive(),
            Primitive::kInt64);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
      Primitive::kInt64));
}

TEST(DriverTest, CanParseFunctionsVariations) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      function hello(int64_t quick) -> int64_t
      function world() -> int64_t
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 2u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetName(), "hello");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetArguments().size(),
            1);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetName(), "world");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetArguments().size(),
            0);
}

TEST(DriverTest, CanParseStruct) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Foo {
        int32_t quick;
        int64_t brown;
        int32_t fox;
      }
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetName(), "Foo");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(), 3);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetPrimitive(),
            Primitive::kInt64);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[2]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetIdentifier(),
            "quick");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetIdentifier(),
            "brown");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[2]
                .GetIdentifier(),
            "fox");
}

TEST(DriverTest, CanParseStructVariants) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Foo {
        int32_t quick;
      }

      struct Bar {
      }

      struct Baz{
        int32_t fox;
        int64_t fox2;
      }
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 3u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetName(), "Foo");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(), 1);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[1].GetName(), "Bar");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[1].GetVariables().size(), 0);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[2].GetName(), "Baz");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[2].GetVariables().size(), 2);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetIdentifier(),
            "quick");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[2]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[2]
                .GetVariables()[1]
                .GetPrimitive(),
            Primitive::kInt64);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[2]
                .GetVariables()[0]
                .GetIdentifier(),
            "fox");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[2]
                .GetVariables()[1]
                .GetIdentifier(),
            "fox2");
}

TEST(DriverTest, CanParseAllPrimitives) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Foo {
        int8_t a1;
        int16_t a2;
        int32_t a3;
        int64_t a4;
        uint8_t a5;
        uint16_t a6;
        uint32_t a7;
        uint64_t a8;
      }
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetName(), "Foo");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(), 8);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kInt8);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetIdentifier(),
            "a1");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetPrimitive(),
            Primitive::kInt16);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetIdentifier(),
            "a2");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[2]
                .GetPrimitive(),
            Primitive::kInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[2]
                .GetIdentifier(),
            "a3");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[3]
                .GetPrimitive(),
            Primitive::kInt64);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[3]
                .GetIdentifier(),
            "a4");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[4]
                .GetPrimitive(),
            Primitive::kUnsignedInt8);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[4]
                .GetIdentifier(),
            "a5");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[5]
                .GetPrimitive(),
            Primitive::kUnsignedInt16);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[5]
                .GetIdentifier(),
            "a6");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[6]
                .GetPrimitive(),
            Primitive::kUnsignedInt32);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[6]
                .GetIdentifier(),
            "a7");
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[7]
                .GetPrimitive(),
            Primitive::kUnsignedInt64);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[7]
                .GetIdentifier(),
            "a8");
}

TEST(DriverTest, CanParsePointers) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Foo {
        int8_t* a1;
      }
      function world() -> int64_t*
      function world2(uint64_t* a) -> int32_t*
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetName(), "Foo");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(), 1);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kInt8);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetIdentifier(),
            "a1");
  ASSERT_TRUE(
      driver.GetNamespaces()[0].GetStructs()[0].GetVariables()[0].IsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 2u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetName(), "world");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetArguments().size(),
            0u);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
      Primitive::kInt64));
  ASSERT_TRUE(driver.GetNamespaces()[0].GetFunctions()[0].ReturnsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetName(), "world2");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetArguments().size(),
            1u);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[1]
                .GetArguments()[0]
                .GetIdentifier(),
            "a");
  ASSERT_TRUE(driver.GetNamespaces()[0]
                  .GetFunctions()[1]
                  .GetArguments()[0]
                  .IsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[1]
                .GetArguments()[0]
                .GetPrimitive(),
            Primitive::kUnsignedInt64);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[1].GetReturnType(),
      Primitive::kInt32));
  ASSERT_TRUE(driver.GetNamespaces()[0].GetFunctions()[1].ReturnsPointer());
}

TEST(DriverTest, CanParseVoidAndVoidPointer) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Foo {
        void* a1;
        // This is illegal but instead of making the grammar more complicated
        // we will error out in the sema pass instead.
        void a2;
      }
      function world() -> void*
      function world2(void* a) -> void*
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetName(), "Foo");
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(), 2);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetPrimitive(),
            Primitive::kVoid);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetIdentifier(),
            "a1");
  ASSERT_TRUE(
      driver.GetNamespaces()[0].GetStructs()[0].GetVariables()[0].IsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetPrimitive(),
            Primitive::kVoid);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[1]
                .GetIdentifier(),
            "a2");
  ASSERT_FALSE(
      driver.GetNamespaces()[0].GetStructs()[0].GetVariables()[1].IsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 2u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetName(), "world");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetArguments().size(),
            0u);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
      Primitive::kVoid));
  ASSERT_TRUE(driver.GetNamespaces()[0].GetFunctions()[0].ReturnsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetName(), "world2");
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetArguments().size(),
            1u);
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[1]
                .GetArguments()[0]
                .GetIdentifier(),
            "a");
  ASSERT_TRUE(driver.GetNamespaces()[0]
                  .GetFunctions()[1]
                  .GetArguments()[0]
                  .IsPointer());
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetFunctions()[1]
                .GetArguments()[0]
                .GetPrimitive(),
            Primitive::kVoid);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[1].GetReturnType(),
      Primitive::kVoid));
  ASSERT_TRUE(driver.GetNamespaces()[0].GetFunctions()[1].ReturnsPointer());
}

TEST(DriverTest, CanParseEnums) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      enum EmptyEnum {

      }

      enum JustOneMember {
       OneMember
      }

      enum SomeMoreMembers {
       Tinker,
       Tailor,
       Soldier,
       Sailor,
       RichMan,
       PoorMan,
       BeggarMan,
       Spy,
      }

      enum SomeMoreMembers2 {
       Tinker,
       Tailor,
       Soldier,
       Sailor,
       RichMan,
       PoorMan,
       BeggarMan,
       Spy
      }

    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums().size(), 4u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[0].GetName(), "EmptyEnum");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[1].GetName(), "JustOneMember");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[1].GetMembers().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[1].GetMembers()[0],
            "OneMember");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetName(),
            "SomeMoreMembers");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers().size(), 8u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[0], "Tinker");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[1], "Tailor");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[2], "Soldier");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[3], "Sailor");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[4], "RichMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[5], "PoorMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[6],
            "BeggarMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[2].GetMembers()[7], "Spy");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetName(),
            "SomeMoreMembers2");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers().size(), 8u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[0], "Tinker");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[1], "Tailor");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[2], "Soldier");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[3], "Sailor");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[4], "RichMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[5], "PoorMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[6],
            "BeggarMan");
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums()[3].GetMembers()[7], "Spy");
}

TEST(DriverTest, EnumsCanBeStructMembers) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      enum ShapeType {
        Rectangle,
        Circle,
        Triangle,
      }

      struct Shape {
        ShapeType type;
      }
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(),
            1u);
  ASSERT_FALSE(driver.GetNamespaces()[0]
                   .GetStructs()[0]
                   .GetVariables()[0]
                   .GetPrimitive()
                   .has_value());
  ASSERT_TRUE(driver.GetNamespaces()[0]
                  .GetStructs()[0]
                  .GetVariables()[0]
                  .GetUserDefinedType()
                  .has_value());
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetUserDefinedType()
                .value(),
            "ShapeType");
  ASSERT_FALSE(
      driver.GetNamespaces()[0].GetStructs()[0].GetVariables()[0].IsPointer());
}

TEST(DriverTest, EnumsCanBeFunctionArguments) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      enum ShapeType {
        Rectangle,
        Circle,
        Triangle,
      }

      function DoSomething(ShapeType type) -> void
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 1u);
}

TEST(DriverTest, StructPointersCanBeStructMembers) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct Shape {
        ShapeType* type;
      }

      struct ShapeType {

      }
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 2u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs()[0].GetVariables().size(),
            1u);
  ASSERT_FALSE(driver.GetNamespaces()[0]
                   .GetStructs()[0]
                   .GetVariables()[0]
                   .GetPrimitive()
                   .has_value());
  ASSERT_TRUE(driver.GetNamespaces()[0]
                  .GetStructs()[0]
                  .GetVariables()[0]
                  .GetUserDefinedType()
                  .has_value());
  ASSERT_EQ(driver.GetNamespaces()[0]
                .GetStructs()[0]
                .GetVariables()[0]
                .GetUserDefinedType()
                .value(),
            "ShapeType");
  ASSERT_TRUE(
      driver.GetNamespaces()[0].GetStructs()[0].GetVariables()[0].IsPointer());
}

TEST(DriverTest, StructPointersCanBeFunctionArguments) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct ShapeType {

      }

      function DoSomething(ShapeType* type) -> void
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 1u);
}

TEST(DriverTest, CanReturnUserDefinedType) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      struct ShapeType {

      }

      enum Foo {

      }

      function DoSomething(ShapeType* type) -> ShapeType*

      function DoSomething2(ShapeType* type) -> Foo
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetStructs().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetEnums().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 2u);
}

TEST(DriverTest, VoidReturnIsOptional) {
  Driver driver;
  auto result = driver.Parse(R"~(

    namespace foo {
      function DoSomething()
      function DoSomething2(int32_t a, int64_t b)
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions().size(), 2u);
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
      Primitive::kVoid));
  ASSERT_FALSE(driver.GetNamespaces()[0].GetFunctions()[0].ReturnsPointer());
  ASSERT_TRUE(CheckFunctionReturnType(
      driver.GetNamespaces()[0].GetFunctions()[1].GetReturnType(),
      Primitive::kVoid));
  ASSERT_FALSE(driver.GetNamespaces()[0].GetFunctions()[1].ReturnsPointer());
}

TEST(DriverTest, ErrorLocationsAreCorrect) {
  Driver driver;
  auto result = driver.Parse(R"~(// Comment
    // More stuff
    // Yet more comments




    namespace foo {
    ;
      function DoSomething()
      function DoSomething2(int32_t a, int64_t b)
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("9:5"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFile1_1) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error1_1.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());

  driver.PrettyPrintErrors(std::cerr);
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("1:1: error"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFile3_3) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error3_3.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());

  driver.PrettyPrintErrors(std::cerr);
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("3:3: error"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFile3_1) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error3_1.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());

  driver.PrettyPrintErrors(std::cerr);
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("3:1: error"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFile51_12) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error51_12.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());

  driver.PrettyPrintErrors(std::cerr);
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("51:12: error"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFile84_24) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error84_24.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());

  driver.PrettyPrintErrors(std::cerr);
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream);
  ASSERT_NE(stream.str().find("84:24: error"), std::string::npos);
}

TEST(DriverTest, ErrorLocationsAreCorrectFilePretty) {
  Driver driver;

  auto source = ReadFileAsString(EPOXY_FIXTURES_LOCATION "error_pretty.epoxy");
  ASSERT_TRUE(source.has_value());
  auto result = driver.Parse(source.value());
  driver.PrettyPrintErrors(std::cerr, source.value());
  std::cerr.flush();
  ASSERT_EQ(result, Driver::ParserResult::kSyntaxError);
  std::stringstream stream;
  driver.PrettyPrintErrors(stream, source.value());
  ASSERT_NE(stream.str().find("-----------------------^"), std::string::npos);
}

}  // namespace testing
}  // namespace epoxy
