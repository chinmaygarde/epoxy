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

  Driver();

  ~Driver();

  ParserResult Parse(const std::string& text);

  const std::vector<Namespace>& GetNamespaces() const;

  void AddNamespace(Namespace ns);

  void PrettyPrintErrors(std::ostream& stream) const;

  // TODO: Accessed by epoxy_lex. This should be a private with a friend
  // function.
  location loc;

  void ReportParsingError(const class location& location,
                          const std::string& message);

 private:
  struct Error {
    class location location;
    std::string message;
  };
  std::vector<Namespace> namespaces_;
  std::vector<Error> errors_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Driver);
};

}  // namespace epoxy
