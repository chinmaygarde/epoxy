#include "sema.h"

#include <map>
#include <set>

namespace epoxy {

Sema::Sema() = default;

Sema::~Sema() = default;

Sema::Result Sema::Perform(std::vector<Namespace> namespaces_vector) {
  std::map<std::string, Namespace> namespaces;

  for (const auto& ns : namespaces_vector) {
    namespaces[ns.GetName()].SetName(ns.GetName());
    namespaces[ns.GetName()].AddFunctions(ns.GetFunctions());
    namespaces[ns.GetName()].AddStructs(ns.GetStructs());
  }

  for (const auto& ns : namespaces) {
    if (!ns.second.PassesSema(errors_)) {
      return Result::kError;
    }
  }

  return Result::kSuccess;
}

void Sema::PrettyPrintErrors(std::ostream& stream) {
  stream << errors_.str() << std::endl;
}

}  // namespace epoxy
