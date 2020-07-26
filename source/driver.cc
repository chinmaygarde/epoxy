// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "driver.h"

#include "scanner.h"

#include <iostream>

namespace epoxy {

Driver::Driver() = default;

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
    stream << "Error: " << error.location << " " << error.message << std::endl;
  }
}

const std::vector<Namespace>& Driver::GetNamespaces() const {
  return namespaces_;
}

}  // namespace epoxy
