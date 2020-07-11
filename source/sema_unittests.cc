#include <gtest/gtest.h>

#include "driver.h"
#include "sema.h"

namespace epoxy {
namespace testing {

TEST(SemaTest, Empty) {
  Driver driver;
  auto driver_result = driver.Parse(R"~()~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kSuccess);
}

TEST(SemaTest, DuplicateFunctionsCauseSemaError) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function foo() -> void;
      function foo(int32_t a, int32_t b) -> void*;
    }
  )~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  sema.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, NonPointerVoidMemberIsError) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        void a;
      }
    }
  )~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  sema.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Sema::Result::kError);
}

}  // namespace testing
}  // namespace epoxy
