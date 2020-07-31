// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "file.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace epoxy {

std::optional<std::string> ReadFileAsString(const std::string& file_path) {
  std::ifstream file_stream;
  file_stream.open(file_path, std::ifstream::in);
  if (file_stream.fail()) {
    std::cerr << "Could not open " << file_path << " for reading." << std::endl;
    return std::nullopt;
  }
  std::stringstream stream;
  stream << file_stream.rdbuf();
  if (!file_stream.good()) {
    std::cerr << "Could not read the whole file " << file_path << std::endl;
    return std::nullopt;
  }
  return HomogenizeNewlines(stream.str());
}

bool OverwriteFileWithStringData(const std::string& file_path,
                                 std::string data) {
  std::ofstream file_stream;
  file_stream.open(file_path, std::ofstream::out | std::ofstream::trunc);
  if (file_stream.fail()) {
    std::cerr << "Could not open " << file_path << " for writing." << std::endl;
    return false;
  }
  file_stream << data;
  if (!file_stream.good()) {
    std::cerr << "Could not write the whole file " << file_path << std::endl;
    return false;
  }
  return true;
}

std::string HomogenizeNewlines(const std::string& string) {
  return StringReplaceAllOccurrances(string, "\r\n", "\n");
}

std::string StringReplaceAllOccurrances(const std::string& string,
                                        const std::string& pattern,
                                        const std::string& replacement) {
  if (pattern.empty()) {
    return string;
  }

  std::stringstream stream;

  size_t offset = 0u;

  while (true) {
    auto found = string.find(pattern, offset);

    // std::string::npos for second argument means copy to end.
    stream << string.substr(offset, found - offset);

    if (found == std::string::npos) {
      break;
    }

    stream << replacement;

    offset = found + pattern.size();
  }

  return stream.str();
}

}  // namespace epoxy
