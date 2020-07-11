#include "types.h"

#include <algorithm>
#include <iterator>
#include <set>

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

bool Variable::PassesSema(std::stringstream& stream) const {
  if (primitive_ == Primitive::kVoid && !is_pointer_) {
    stream << "Variable '" << identifier_ << "' cannot be void." << std::endl;
    return false;
  }
  return true;
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

bool Function::PassesSema(std::stringstream& stream) const {
  for (const auto& arg : arguments_) {
    if (!arg.PassesSema(stream)) {
      return false;
    }
  }
  return true;
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

void Namespace::SetName(const std::string& name) {
  name_ = std::move(name);
}

const std::vector<Function>& Namespace::GetFunctions() const {
  return functions_;
}

const std::vector<Struct>& Namespace::GetStructs() const {
  return structs_;
}

void Namespace::AddFunctions(const std::vector<Function>& functions) {
  std::copy(functions.cbegin(), functions.cend(),
            std::back_inserter(functions_));
}

void Namespace::AddStructs(const std::vector<Struct>& structs) {
  std::copy(structs.cbegin(), structs.cend(), std::back_inserter(structs_));
}

bool Namespace::HasDuplicateFunctions(std::stringstream& stream) const {
  std::set<std::string> function_names;
  for (const auto& function : functions_) {
    const auto& function_name = function.GetName();
    auto found = function_names.find(function_name);
    if (found == function_names.end()) {
      function_names.insert(function_name);
    } else {
      stream << "Duplicate function '" << function_name << "' in namespace '"
             << name_ << "'" << std::endl;
      return false;
    }
  }
  return true;
}

bool Namespace::HasDuplicateStructs(std::stringstream& stream) const {
  std::set<std::string> struct_names;
  for (const auto& strut : structs_) {
    const auto& struct_name = strut.GetName();
    auto found = struct_names.find(struct_name);
    if (found == struct_names.end()) {
      struct_names.insert(struct_name);
    } else {
      stream << "Duplicate struct '" << struct_name << "' in namespace '"
             << name_ << "'" << std::endl;
      return false;
    }
  }
  return true;
}

bool Namespace::PassesSema(std::stringstream& stream) const {
  if (!HasDuplicateFunctions(stream)) {
    return false;
  }

  if (!HasDuplicateStructs(stream)) {
    return false;
  }

  for (const auto& strut : structs_) {
    if (!strut.PassesSema(stream)) {
      return false;
    }
  }

  for (const auto& func : functions_) {
    if (!func.PassesSema(stream)) {
      return false;
    }
  }

  return true;
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

bool Struct::PassesSema(std::stringstream& stream) const {
  std::set<std::string> variable_names;
  for (const auto& var : variables_) {
    if (!var.PassesSema(stream)) {
      return false;
    }

    const auto& variable_name = var.GetIdentifier();
    auto found = variable_names.find(variable_name);
    if (found == variable_names.end()) {
      variable_names.insert(variable_name);
    } else {
      stream << "Duplicate variable '" << variable_name << "' in struct named '"
             << name_ << "'." << std::endl;
      return false;
    }
  }
  return true;
}

}  // namespace epoxy
