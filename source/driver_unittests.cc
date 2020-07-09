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
      function say_hello(int32_t a, int64_t b) -> int32_t
    }

    )~");

  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Driver::ParserResult::kSuccess);
  ASSERT_EQ(driver.GetNamespaces().size(), 1u);
}

}  // namespace testing
}  // namespace epoxy
