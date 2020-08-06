// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "macros.h"

namespace epoxy {

class CommandLine {
 public:
  CommandLine(int argc, const char* argv[]);

  CommandLine(std::vector<std::string> args);

  ~CommandLine();

  std::optional<std::string> GetString(const std::string& key) const;

  std::optional<bool> GetOption(const std::string& key) const;

  bool GetOptionWithDefault(const std::string& key, bool def) const;

 private:
  std::vector<std::string> args_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(CommandLine);
};

}  // namespace epoxy
