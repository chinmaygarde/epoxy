// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include "location.hh"
#include "types.h"

#include <vector>

namespace epoxy {

class Driver {
 public:
  enum class ParserResult {
    kSuccess,
    kSyntaxError,
    kParserError,
    kOutOfMemory,
  };

  Driver(std::string advisory_file_name = "main.epoxy");

  ~Driver();

  ParserResult Parse(const std::string& text);

  const std::vector<Namespace>& GetNamespaces() const;

  void AddNamespace(Namespace ns);

  void PrettyPrintErrors(std::ostream& stream,
                         const std::string& original_text = "") const;

  location GetCurrentLocation() const;

  void BumpCurrentLocation(const char* text);

  void ReportParsingError(const class location& location,
                          const std::string& message);

 private:
  struct Error {
    class location location;
    std::string message;
  };
  std::vector<Namespace> namespaces_;
  std::vector<Error> errors_;
  std::string advisory_file_name_;
  location location_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Driver);
};

}  // namespace epoxy
