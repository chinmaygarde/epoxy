#pragma once

#include <sstream>
#include <vector>

#include "macros.h"
#include "types.h"

namespace epoxy {

class Sema {
 public:
  enum class Result {
    kSuccess,
    kError,
  };

  Sema();

  ~Sema();

  Result Perform(std::vector<Namespace> namespaces);

  void PrettyPrintErrors(std::ostream& stream);

  const std::vector<Namespace>& GetNamespaces() const;

 private:
  std::stringstream errors_;
  std::vector<Namespace> namespaces_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Sema);
};

}  // namespace epoxy
