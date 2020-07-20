// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

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
    namespaces[ns.GetName()].AddEnums(ns.GetEnums());
  }

  for (const auto& ns : namespaces) {
    if (!ns.second.PassesSema(errors_)) {
      return Result::kError;
    }
  }

  for (const auto& ns : namespaces) {
    namespaces_.push_back(ns.second);
  }

  return Result::kSuccess;
}

void Sema::PrettyPrintErrors(std::ostream& stream) {
  stream << errors_.str() << std::endl;
}

const std::vector<Namespace>& Sema::GetNamespaces() const {
  return namespaces_;
}

}  // namespace epoxy
