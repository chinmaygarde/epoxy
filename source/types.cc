#include "types.h"

namespace epoxy {

Argument::Argument() = default;

Argument::Argument(Primitive primitive, std::string identifier)
    : primitive_(std::move(primitive)), identifier_(std::move(identifier)) {}

Argument::~Argument() = default;

Function::Function() = default;

Function::Function(std::string name,
                   std::vector<Argument> arguments,
                   Primitive return_type)
    : name_(std::move(name)),
      arguments_(std::move(arguments)),
      return_type_(std::move(return_type)) {}

Function::~Function() = default;

Namespace::Namespace() = default;

Namespace::Namespace(std::string name, NamespaceItems items)
    : name_(std::move(name)) {}

Namespace::~Namespace() = default;

const std::string& Namespace::GetName() const {
  return name_;
}

}  // namespace epoxy
