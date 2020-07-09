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

}  // namespace testing
}  // namespace epoxy
