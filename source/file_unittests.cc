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

TEST(FileTest, CanReplaceAllOccurances) {
  {
    const std::string original = "RRRR";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = "RRRR";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = "";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = "";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = "RRR;";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = "RRR.";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = "RRR;R";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = "RRR.R";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";RRR;R";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = ".RRR.R";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";RRR;R;AAA";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = ".RRR.R.AAA";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";RRR;R;AAA;;;;";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = ".RRR.R.AAA....";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = "RRR;R;AAA;;;;";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = "RRR.R.AAA....";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";RRR;";
    auto replaced = StringReplaceAllOccurrances(original, ";", ".");
    const std::string asserted = ".RRR.";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";;;RRR;;;";
    auto replaced = StringReplaceAllOccurrances(original, ";;;", ".");
    const std::string asserted = ".RRR.";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";;;RRR;;RRRRAAS.::;;;";
    auto replaced = StringReplaceAllOccurrances(original, ";;;", ".");
    const std::string asserted = ".RRR;;RRRRAAS.::.";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";;;RRR;;RRRRAAS.::;;;";
    auto replaced = StringReplaceAllOccurrances(original, ";;;", ".z");
    const std::string asserted = ".zRRR;;RRRRAAS.::.z";
    ASSERT_EQ(replaced, asserted);
  }
}

TEST(FileTest, CanHomogenizeNewlines) {
  {
    const std::string original = ";;;RRR;;RRRRAAS\n\r\n.::;;;";
    auto replaced = HomogenizeNewlines(original);
    const std::string asserted = ";;;RRR;;RRRRAAS\n\n.::;;;";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";;;RRR;;RRRRAAS\r\n.::;;;";
    auto replaced = HomogenizeNewlines(original);
    const std::string asserted = ";;;RRR;;RRRRAAS\n.::;;;";
    ASSERT_EQ(replaced, asserted);
  }

  {
    const std::string original = ";;;RRR;;RRRRAAS.::;;;\n";
    auto replaced = HomogenizeNewlines(original);
    const std::string asserted = ";;;RRR;;RRRRAAS.::;;;\n";
    ASSERT_EQ(replaced, asserted);
  }
}

}  // namespace testing
}  // namespace epoxy
