// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "driver.h"

#include "file.h"
#include "scanner.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

namespace epoxy {

Driver::Driver(std::string advisory_file_name)
    : advisory_file_name_(std::move(advisory_file_name)) {
  location_.initialize(&advisory_file_name_);
}

Driver::~Driver() = default;

void Driver::AddNamespace(Namespace ns) {
  namespaces_.emplace_back(std::move(ns));
}

Driver::ParserResult Driver::Parse(const std::string& text) {
  Scanner scanner(text);

  if (!scanner.IsValid()) {
    return ParserResult::kParserError;
  }

  Parser parser(*this, scanner.GetHandle());

  switch (parser.parse()) {
    case 0: /* parsing was successful (return is due to end-of-input) */
      return ParserResult::kSuccess;
    case 1: /* contains a syntax error */
      return ParserResult::kSyntaxError;
    case 2: /* memory exhaustion */
      return ParserResult::kOutOfMemory;
  }

  return ParserResult::kParserError;
}

void Driver::ReportParsingError(const class location& location,
                                const std::string& message) {
  errors_.emplace_back(Driver::Error{location, message});
}

static void UnderscoreErrorInText(std::ostream& stream,
                                  const location& position,
                                  const std::string& original_text) {
  if (original_text.empty()) {
    return;
  }
  auto line = GetLineInString(original_text, position.begin.line);
  if (!line.has_value()) {
    return;
  }

  std::string pad(2u, ' ');

  stream << pad << line.value() << std::endl;

  const auto column =
      std::max<size_t>(static_cast<size_t>(position.begin.column), 1u);

  std::string bar(column - 1, '-');

  stream << pad << bar << "^" << std::endl;
}

void Driver::PrettyPrintErrors(std::ostream& stream,
                               const std::string& original_text) const {
  for (const auto& error : errors_) {
    const auto& error_begin = error.location.begin;
    stream << *error_begin.filename << ":" << error_begin.line << ":"
           << error_begin.column << ": error: " << error.message << std::endl;
    UnderscoreErrorInText(stream, error.location, original_text);
  }
}

const std::vector<Namespace>& Driver::GetNamespaces() const {
  return namespaces_;
}

location Driver::GetCurrentLocation() const {
  return location_;
}

static size_t GetNewlinesInString(const std::string& string) {
  return std::count_if(string.begin(), string.end(),
                       [](const auto& c) { return c == '\n'; });
}

static size_t CharsAfterLastNewline(const std::string& string) {
  auto last = string.find_last_of('\n');
  if (last == std::string::npos) {
    return string.size();
  } else {
    return string.size() - last - 1u;
  }
}

void Driver::BumpCurrentLocation(const char* c_text) {
  std::string text(c_text);

  location_.step();

  location_.lines(GetNewlinesInString(text));
  location_.columns(CharsAfterLastNewline(text));
}

}  // namespace epoxy
