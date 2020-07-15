// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "file.h"

#include <fstream>
#include <sstream>

namespace epoxy {

std::optional<std::string> ReadFileAsString(const std::string& file_path) {
  std::ifstream file_stream;
  file_stream.open(file_path, std::ifstream ::in);
  if (!file_stream.good()) {
    return std::nullopt;
  }
  std::stringstream stream;
  stream << file_stream.rdbuf();
  file_stream.clear();
  return stream.str();
}

bool OverwriteFileWithStringData(const std::string& file_path,
                                 std::string data) {
  std::ofstream file_stream;
  file_stream.open(file_path, std::ofstream::out | std::ofstream::trunc);
  if (!file_stream.good()) {
    return false;
  }
  file_stream << data;
  file_stream.close();
  return true;
}

}  // namespace epoxy
