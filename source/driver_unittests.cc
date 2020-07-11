#include <gtest/gtest.h>

#include "driver.h"

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
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
            Primitive::kInt64);
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
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
            Primitive::kInt64);
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
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetReturnType(),
            Primitive::kInt32);
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
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[0].GetReturnType(),
            Primitive::kVoid);
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
  ASSERT_EQ(driver.GetNamespaces()[0].GetFunctions()[1].GetReturnType(),
            Primitive::kVoid);
  ASSERT_TRUE(driver.GetNamespaces()[0].GetFunctions()[1].ReturnsPointer());
}

}  // namespace testing
}  // namespace epoxy
