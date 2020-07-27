// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "code_gen.h"
#include "driver.h"
#include "sema.h"

namespace epoxy {
namespace testing {

TEST(CodeGenTest, Simple) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        void* a1;
        int32_t a2;
      }
      function world() -> int32_t
      function world2(void* a) -> void*
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kSuccess);
  auto code_gen = CodeGen({});
  auto code_gen_result = code_gen.Render(sema.GetNamespaces());
  ASSERT_TRUE(code_gen_result.result.has_value());
}

TEST(CodeGenTest, CanCreateJSONDump) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        void* a1;
        int32_t a2;
      }
      function world() -> int32_t
      function world2(void* a) -> void*
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kSuccess);
  auto code_gen = CodeGen({});
  auto json_dump = code_gen.GenerateTemplateDataJSON(sema.GetNamespaces());
  ASSERT_NE(json_dump.find("epoxy_version"), std::string::npos);
}

}  // namespace testing
}  // namespace epoxy
