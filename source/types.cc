#include "types.h"

namespace epoxy {

Argument::Argument() = default;

Argument::Argument(Primitive primitive, std::string identifier)
    : primitive_(std::move(primitive)), identifier_(std::move(identifier)) {}

Argument::~Argument() = default;

Primitive Argument::GetPrimitive() const {
  return primitive_;
}

const std::string& Argument::GetIdentifier() const {
  return identifier_;
}

Function::Function() = default;

Function::Function(std::string name,
                   std::vector<Argument> arguments,
                   Primitive return_type)
    : name_(std::move(name)),
      arguments_(std::move(arguments)),
      return_type_(std::move(return_type)) {}

Function::~Function() = default;

const std::string& Function::GetName() const {
  return name_;
}

const std::vector<Argument>& Function::GetArguments() const {
  return arguments_;
}

Primitive Function::GetReturnType() const {
  return return_type_;
}

Namespace::Namespace() = default;

Namespace::Namespace(std::string name, NamespaceItems items)
    : name_(std::move(name)) {
  for (const auto& item : items) {
    if (auto function = std::get_if<Function>(&item)) {
      functions_.push_back(*function);
    }

    if (auto struct_item = std::get_if<Struct>(&item)) {
      structs_.push_back(*struct_item);
    }
  }
}

Namespace::~Namespace() = default;

const std::string& Namespace::GetName() const {
  return name_;
}

const std::vector<Function>& Namespace::GetFunctions() const {
  return functions_;
}

const std::vector<Struct>& Namespace::GetStructs() const {
  return structs_;
}

Struct::Struct() = default;

Struct::Struct(std::string name, std::vector<Argument> variables)
    : name_(std::move(name)), variables_(std::move(variables)) {}

Struct::~Struct() = default;

const std::string& Struct::GetName() const {
  return name_;
}

const std::vector<Argument> Struct::GetVariables() const {
  return variables_;
}

}  // namespace epoxy
