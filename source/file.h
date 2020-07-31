// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include <optional>
#include <string>

#pragma once

namespace epoxy {

std::optional<std::string> ReadFileAsString(const std::string& file_path);

bool OverwriteFileWithStringData(const std::string& file_path,
                                 std::string data);

std::string HomogenizeNewlines(const std::string& string);

std::string StringReplaceAllOccurrances(const std::string& string,
                                        const std::string& pattern,
                                        const std::string& replacement);

std::optional<std::string> GetLineInString(const std::string& string,
                                           size_t line);

}  // namespace epoxy
