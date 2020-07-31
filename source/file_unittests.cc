// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include <gtest/gtest.h>
#include <string>

#include "file.h"
#include "fixture.h"

namespace epoxy {
namespace testing {

TEST(FileTest, CanReadFileAsString) {
  auto string = ReadFileAsString(EPOXY_FIXTURES_LOCATION "hello.txt");
  ASSERT_TRUE(string.has_value());
  ASSERT_NE(string.value().find("hello"), std::string::npos);
}

}  // namespace testing
}  // namespace epoxy
