// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

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
      function foo() -> void
      function foo(int32_t a, int32_t b) -> void*
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

TEST(SemaTest, EnumCannotHaveDuplicateMembers) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum Foo {
        SomeMember,
        SomeMember,
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

TEST(SemaTest, StructsAndEnumNamesCannotCollide) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum Foo {

      }
      struct Foo {

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

TEST(SemaTest, EnumNamesInFunctionArgsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function Foo(AbsentEnum val) -> void
    }
  )~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  sema.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, EnumNamesInStructsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        AbsentEnum val;
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

TEST(SemaTest, StructPointersInStructsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        AbsentStrct* val;
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

TEST(SemaTest, StructNamesInFunctionArgsMustBeKnown) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      function Foo(AbsentStruct* val) -> void
    }
  )~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  sema.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, CannotReturnPointerToUnknownStruct) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      enum AbsentStruct {

      }
      function Foo() -> AbsentStruct*
    }
  )~");
  driver.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  sema.PrettyPrintErrors(std::cerr);
  ASSERT_EQ(result, Sema::Result::kError);
}

TEST(SemaTest, CannotReturnUnknownEnum) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct AbsentEnum {

      }
      function Foo() -> AbsentEnum
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
