#pragma once

#include <vector>

#include "macros.h"
#include "types.h"

namespace epoxy {

class Sema {
 public:
  enum class SemaResult {
    kSuccess,
    kError,
  };

  Sema();

  ~Sema();

  SemaResult Perform(std::vector<Namespace> namespaces);

 private:
  std::vector<std::string> errors_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Sema);
};

}  // namespace epoxy
