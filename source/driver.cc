// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "driver.h"

#include "scanner.h"

#include <iostream>
#include <sstream>
#include <string>

namespace epoxy {

Driver::Driver(std::string advisory_file_name)
    : advisory_file_name_(std::move(advisory_file_name)) {
  location_.initialize(&advisory_file_name_, 1u, 1u);
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

void Driver::PrettyPrintErrors(std::ostream& stream) const {
  for (const auto& error : errors_) {
    const auto& error_begin = error.location.begin;
    stream << *error_begin.filename << ":" << error_begin.line << ":"
           << error_begin.column << " Error: " << error.message << std::endl;
  }
}

const std::vector<Namespace>& Driver::GetNamespaces() const {
  return namespaces_;
}

location Driver::GetCurrentLocation() const {
  return location_;
}

void Driver::BumpCurrentLocation(const char* c_text) {
  std::string text(c_text);
  std::stringstream stream(text);
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(stream, line)) {
    lines.emplace_back(std::move(line));
  }
  if (lines.empty()) {
    return;
  }
  location_.step();
  if (lines.size() > 1u) {
    // A newline was inserted.
    location_.lines(lines.size() - 1u);
    location_.columns(lines.back().size());
  } else {
    // An existing line was extended.
    location_.columns(text.size());
  }
}

}  // namespace epoxy
