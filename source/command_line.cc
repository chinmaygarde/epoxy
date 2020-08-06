// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "command_line.h"

#include <algorithm>
#include <optional>

namespace epoxy {

static std::vector<std::string> ArgCArgVToStringArgs(int argc,
                                                     const char* argv[]) {
  std::vector<std::string> arguments;
  arguments.reserve(argc);
  for (int i = 1; i < argc; i++) {
    arguments.push_back(argv[i]);
  }
  return arguments;
}

CommandLine::CommandLine(int argc, const char* argv[])
    : CommandLine(ArgCArgVToStringArgs(argc, argv)) {}

CommandLine::CommandLine(std::vector<std::string> args)
    : args_(std::move(args)) {}

CommandLine::~CommandLine() = default;

std::optional<std::string> CommandLine::GetString(
    const std::string& key) const {
  auto flag = "--" + key;
  for (size_t i = 0; i < args_.size(); i++) {
    if (args_[i] == flag && i + 1 < args_.size()) {
      return args_[i + 1];
    }
  }
  return std::nullopt;
}

std::optional<bool> CommandLine::GetOption(const std::string& key) const {
  const auto true_flag = "--" + key;
  const auto false_flag = "--no-" + key;
  auto found_true =
      std::find_if(args_.begin(), args_.end(),
                   [&](const auto& val) { return val == true_flag; });
  auto found_false =
      std::find_if(args_.begin(), args_.end(),
                   [&](const auto& val) { return val == false_flag; });
  if (found_true != args_.end() && found_false != args_.end()) {
    return std::nullopt;
  }
  if (found_true != args_.end()) {
    return true;
  }
  if (found_false != args_.end()) {
    return false;
  }
  return std::nullopt;
}

bool CommandLine::GetOptionWithDefault(const std::string& key, bool def) const {
  auto val = GetOption(key);
  if (val.has_value()) {
    return val.value();
  }
  return def;
}

}  // namespace epoxy
