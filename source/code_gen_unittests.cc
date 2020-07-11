#include <gtest/gtest.h>

#include "driver.h"
#include "sema.h"

namespace epoxy {
namespace testing {

TEST(CodeGenDartTest, Simple) {
  Driver driver;
  auto driver_result = driver.Parse(R"~(
    namespace foo {
      struct Foo {
        void* a1;
        int32_t a2;
      }
      function world() -> int32_t;
      function world2(void* a) -> void*;
    }
  )~");
  ASSERT_EQ(driver_result, Driver::ParserResult::kSuccess);
  Sema sema;
  auto result = sema.Perform(driver.GetNamespaces());
  ASSERT_EQ(result, Sema::Result::kSuccess);
}

}  // namespace testing
}  // namespace epoxy
