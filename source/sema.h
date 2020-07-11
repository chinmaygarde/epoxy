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

 private:
  std::stringstream errors_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Sema);
};

}  // namespace epoxy
