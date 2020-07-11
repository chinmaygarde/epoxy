#include "sema.h"

namespace epoxy {

Sema::Sema() = default;

Sema::~Sema() = default;

Sema::SemaResult Sema::Perform(std::vector<Namespace> namespaces) {
  return SemaResult::kError;
}

}  // namespace epoxy
