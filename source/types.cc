#include "types.h"

namespace epoxy {

Variable::Variable() = default;

Variable::Variable(Primitive primitive, std::string identifier, bool is_pointer)
    : primitive_(std::move(primitive)),
      identifier_(std::move(identifier)),
      is_pointer_(is_pointer) {}

Variable::~Variable() = default;

Primitive Variable::GetPrimitive() const {
  return primitive_;
}

const std::string& Variable::GetIdentifier() const {
  return identifier_;
}

bool Variable::IsPointer() const {
  return is_pointer_;
}

Function::Function() = default;

Function::Function(std::string name,
                   std::vector<Variable> arguments,
                   Primitive return_type,
                   bool pointer_return)
    : name_(std::move(name)),
      arguments_(std::move(arguments)),
      return_type_(std::move(return_type)),
      pointer_return_(pointer_return) {}

Function::~Function() = default;

const std::string& Function::GetName() const {
  return name_;
}

const std::vector<Variable>& Function::GetArguments() const {
  return arguments_;
}

Primitive Function::GetReturnType() const {
  return return_type_;
}

bool Function::ReturnsPointer() const {
  return pointer_return_;
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

Struct::Struct(std::string name, std::vector<Variable> variables)
    : name_(std::move(name)), variables_(std::move(variables)) {}

Struct::~Struct() = default;

const std::string& Struct::GetName() const {
  return name_;
}

const std::vector<Variable>& Struct::GetVariables() const {
  return variables_;
}

}  // namespace epoxy
