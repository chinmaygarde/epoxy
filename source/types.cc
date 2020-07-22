// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "types.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <set>

namespace epoxy {

Variable::Variable() = default;

Variable::Variable(Primitive primitive, std::string identifier, bool is_pointer)
    : type_(std::move(primitive)),
      identifier_(std::move(identifier)),
      is_pointer_(is_pointer) {}

Variable::Variable(std::string type, std::string identifier, bool is_pointer)
    : type_(std::move(type)),
      identifier_(std::move(identifier)),
      is_pointer_(is_pointer) {}

Variable::~Variable() = default;

const std::string& Variable::GetIdentifier() const {
  return identifier_;
}

bool Variable::IsPointer() const {
  return is_pointer_;
}

std::optional<std::string> Variable::GetUserDefinedType() const {
  if (auto user_type = std::get_if<std::string>(&type_)) {
    return *user_type;
  }

  return std::nullopt;
}

std::optional<Primitive> Variable::GetPrimitive() const {
  if (auto prim = std::get_if<Primitive>(&type_)) {
    return *prim;
  }

  return std::nullopt;
}

bool Variable::PassesSema(const Namespace& ns,
                          std::stringstream& stream) const {
  if (auto primitive = GetPrimitive(); primitive.has_value()) {
    if (primitive == Primitive::kVoid && !is_pointer_) {
      stream << "Variable '" << identifier_ << "' cannot be void." << std::endl;
      return false;
    }
  }

  if (auto user_type = GetUserDefinedType(); user_type.has_value()) {
    if (IsPointer()) {
      // If the user defined type is a pointer, it must be a known struct.
      if (!ns.HasStructNamed(user_type.value())) {
        stream << "No struct named " << user_type.value() << " in namespace "
               << ns.GetName() << "." << std::endl;
        if (ns.HasEnumNamed(user_type.value())) {
          stream << "There is an enum named " << user_type.value()
                 << " but enums but may only be specified by value."
                 << std::endl;
        }
        return false;
      }
    } else {
      // If the user defined type is not a pointer, it must be a known enum.
      if (!ns.HasEnumNamed(user_type.value())) {
        stream << "No enum named " << user_type.value() << " in namespace "
               << ns.GetName() << "." << std::endl;
        if (ns.HasStructNamed(user_type.value())) {
          stream << "There is an struct named " << user_type.value()
                 << " but structs may not be specified by value. Use a pointer "
                    "to the struct instead."
                 << std::endl;
        }
        return false;
      }
    }
  }

  return true;
}

static std::string PrimitiveToTypeString(Primitive primitive) {
  switch (primitive) {
    case Primitive::kVoid:
      return "void";
    case Primitive::kInt8:
      return "int8_t";
    case Primitive::kInt16:
      return "int16_t";
    case Primitive::kInt32:
      return "int32_t";
    case Primitive::kInt64:
      return "int64_t";
    case Primitive::kUnsignedInt8:
      return "uint8_t";
    case Primitive::kUnsignedInt16:
      return "uint16_t";
    case Primitive::kUnsignedInt32:
      return "uint32_t";
    case Primitive::kUnsignedInt64:
      return "uint64_t";
    case Primitive::kDouble:
      return "double";
    case Primitive::kFloat:
      return "float";
  };
  return "unknown";
}

nlohmann::json::object_t Variable::GetJSONObject(const Namespace& ns) const {
  nlohmann::json::object_t var;

  if (auto primitive = GetPrimitive(); primitive.has_value()) {
    var["type"] = PrimitiveToTypeString(primitive.value());
    var["is_enum"] = false;
    var["is_struct"] = false;
    var["is_primitive"] = true;
  }

  if (auto user_type = GetUserDefinedType(); user_type.has_value()) {
    var["type"] = user_type.value();
    var["is_enum"] = ns.HasEnumNamed(user_type.value());
    var["is_struct"] = ns.HasStructNamed(user_type.value());
    var["is_primitive"] = false;
  }

  var["identifier"] = identifier_;
  var["is_pointer"] = is_pointer_;
  return var;
}

Function::Function() = default;

Function::Function(std::string name,
                   std::vector<Variable> arguments,
                   ReturnType return_type,
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

Function::ReturnType Function::GetReturnType() const {
  return return_type_;
}

bool Function::ReturnsPointer() const {
  return pointer_return_;
}

bool Function::PassesSema(const Namespace& ns,
                          std::stringstream& stream) const {
  for (const auto& arg : arguments_) {
    if (!arg.PassesSema(ns, stream)) {
      return false;
    }
  }

  if (auto ret = GetUserDefinedReturn(); ret.has_value()) {
    // If the user defined type is a struct, it must be a pointer. Otherwise, it
    // must be an enum.
    if (ReturnsPointer()) {
      if (!ns.HasStructNamed(ret.value())) {
        stream << "Function " << name_ << " in namespace " << ns.GetName()
               << " specifies a return type " << ret.value() << "."
               << std::endl;
        stream << "However, " << ret.value() << " is not a known struct name."
               << std::endl;
        if (ns.HasEnumNamed(ret.value())) {
          stream << "There is an enum named " << ret.value()
                 << ". But enums may only be returned by value. Drop the "
                    "return by pointer."
                 << std::endl;
        }
        return false;
      }
    } else {
      if (!ns.HasEnumNamed(ret.value())) {
        stream << "Function " << name_ << " in namespace " << ns.GetName()
               << " specifies a return type " << ret.value() << "."
               << std::endl;
        stream << "However, " << ret.value() << " is not a known enum name."
               << std::endl;
        if (ns.HasStructNamed(ret.value())) {
          stream << "There is a struct named " << ret.value()
                 << ". But structs may not be returned by value. Use a pointer "
                    "return instead."
                 << std::endl;
        }
        return false;
      }
    }
  }

  return true;
}

std::optional<Primitive> Function::GetPrimitiveReturn() const {
  if (auto val = std::get_if<Primitive>(&return_type_)) {
    return *val;
  }
  return std::nullopt;
}

std::optional<std::string> Function::GetUserDefinedReturn() const {
  if (auto val = std::get_if<std::string>(&return_type_)) {
    return *val;
  }
  return std::nullopt;
}

nlohmann::json::object_t Function::GetJSONObject(const Namespace& ns) const {
  auto args = nlohmann::json::array_t{};

  for (const auto& arg : arguments_) {
    args.emplace_back(arg.GetJSONObject(ns));
  }

  nlohmann::json::object_t fun;
  fun["name"] = name_;
  if (auto ret = GetPrimitiveReturn(); ret.has_value()) {
    fun["return_type"] = PrimitiveToTypeString(ret.value());
    fun["returns_struct"] = false;
    fun["returns_enum"] = false;
    fun["returns_primitive"] = true;
  } else if (auto ret = GetUserDefinedReturn(); ret.has_value()) {
    fun["return_type"] = ret.value();
    fun["returns_struct"] = ns.HasStructNamed(ret.value());
    fun["returns_enum"] = ns.HasEnumNamed(ret.value());
    fun["returns_primitive"] = false;
  }
  fun["pointer_return"] = pointer_return_;
  fun["arguments"] = std::move(args);

  return fun;
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

    if (auto enum_item = std::get_if<Enum>(&item)) {
      enums_.push_back(*enum_item);
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

const std::vector<Enum>& Namespace::GetEnums() const {
  return enums_;
}

bool Namespace::HasEnumNamed(const std::string& name) const {
  return std::find_if(enums_.begin(), enums_.end(), [&](const auto& enumm) {
           return enumm.GetName() == name;
         }) != enums_.end();
}

bool Namespace::HasStructNamed(const std::string& name) const {
  return std::find_if(structs_.begin(), structs_.end(), [&](const auto& strut) {
           return strut.GetName() == name;
         }) != structs_.end();
}

void Namespace::AddFunctions(const std::vector<Function>& functions) {
  std::copy(functions.cbegin(), functions.cend(),
            std::back_inserter(functions_));
}

void Namespace::AddStructs(const std::vector<Struct>& structs) {
  std::copy(structs.cbegin(), structs.cend(), std::back_inserter(structs_));
}

void Namespace::AddEnums(const std::vector<Enum>& enums) {
  std::copy(enums.cbegin(), enums.cend(), std::back_inserter(enums_));
}

bool Namespace::CheckDuplicateFunctions(std::stringstream& stream) const {
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

bool Namespace::CheckStructEnumNameCollisions(std::stringstream& stream) const {
  std::map<std::string, size_t> name_counts;
  auto count_fn = [&](const std::string& name) -> void { name_counts[name]++; };
  std::for_each(structs_.begin(), structs_.end(),
                [&](const auto& str) { count_fn(str.GetName()); });
  std::for_each(enums_.begin(), enums_.end(),
                [&](const auto& enm) { count_fn(enm.GetName()); });
  for (const auto& count : name_counts) {
    if (count.second > 1) {
      stream << "Struct or enum named " << count.first
             << " declared more than once." << std::endl;
      return false;
    }
  }
  return true;
}

bool Namespace::PassesSema(std::stringstream& stream) const {
  if (!CheckDuplicateFunctions(stream)) {
    return false;
  }

  if (!CheckStructEnumNameCollisions(stream)) {
    return false;
  }

  for (const auto& strut : structs_) {
    if (!strut.PassesSema(*this, stream)) {
      return false;
    }
  }

  for (const auto& func : functions_) {
    if (!func.PassesSema(*this, stream)) {
      return false;
    }
  }

  for (const auto& enumm : enums_) {
    if (!enumm.PassesSema(*this, stream)) {
      return false;
    }
  }

  return true;
}

nlohmann::json::object_t Namespace::GetJSONObject() const {
  nlohmann::json::object_t ns;

  auto structs = nlohmann::json::array_t{};
  auto funcs = nlohmann::json::array_t{};
  auto enums = nlohmann::json::array_t{};

  for (const auto& str : structs_) {
    structs.emplace_back(str.GetJSONObject(*this));
  }

  for (const auto& fun : functions_) {
    funcs.emplace_back(fun.GetJSONObject(*this));
  }

  for (const auto& enumm : enums_) {
    enums.emplace_back(enumm.GetJSONObject());
  }

  ns["name"] = name_;
  ns["functions"] = std::move(funcs);
  ns["structs"] = std::move(structs);
  ns["enums"] = std::move(enums);

  return ns;
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

bool Struct::PassesSema(const Namespace& ns, std::stringstream& stream) const {
  std::set<std::string> variable_names;
  for (const auto& var : variables_) {
    if (!var.PassesSema(ns, stream)) {
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

nlohmann::json::object_t Struct::GetJSONObject(const Namespace& ns) const {
  auto vars = nlohmann::json::array_t{};
  for (const auto& var : variables_) {
    vars.emplace_back(var.GetJSONObject(ns));
  }
  nlohmann::json::object_t strut;
  strut["name"] = name_;
  strut["variables"] = std::move(vars);
  return strut;
}

Enum::Enum() = default;

Enum::Enum(std::string name, std::vector<std::string> members)
    : name_(std::move(name)), members_(std::move(members)) {}

Enum::~Enum() = default;

const std::string& Enum::GetName() const {
  return name_;
}

const std::vector<std::string>& Enum::GetMembers() const {
  return members_;
}

bool Enum::PassesSema(const Namespace& ns, std::stringstream& stream) const {
  std::map<std::string, size_t> member_counts;
  for (const auto& member : members_) {
    member_counts[member]++;
  }

  for (const auto& count : member_counts) {
    if (count.second > 1) {
      stream << "Enum " << name_ << " has duplicate member " << count.first;
      return false;
    }
  }
  return true;
}

nlohmann::json::object_t Enum::GetJSONObject() const {
  auto members = nlohmann::json::array_t{};
  for (const auto& member : members_) {
    members.push_back(member);
  }

  nlohmann::json::object_t enumm;
  enumm["name"] = name_;
  enumm["members"] = std::move(members);
  return enumm;
}

}  // namespace epoxy
