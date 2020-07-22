// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "macros.h"

namespace epoxy {

class Namespace;

enum class Primitive {
  kVoid,
  kInt8,
  kInt16,
  kInt32,
  kInt64,
  kUnsignedInt8,
  kUnsignedInt16,
  kUnsignedInt32,
  kUnsignedInt64,
  kDouble,
  kFloat,
};

class Variable {
 public:
  Variable();

  Variable(Primitive primitive, std::string identifier, bool is_pointer);

  Variable(std::string type, std::string identifier, bool is_pointer);

  ~Variable();

  std::optional<Primitive> GetPrimitive() const;

  std::optional<std::string> GetUserDefinedType() const;

  const std::string& GetIdentifier() const;

  bool IsPointer() const;

  bool PassesSema(const Namespace& ns, std::stringstream& stream) const;

  nlohmann::json::object_t GetJSONObject(const Namespace& ns) const;

 private:
  using Type = std::variant<Primitive, std::string>;
  Type type_;
  std::string identifier_;
  bool is_pointer_ = false;
};

class Function {
 public:
  using ReturnType = std::variant<Primitive, std::string>;

  Function();

  Function(std::string name,
           std::vector<Variable> arguments,
           ReturnType return_type,
           bool pointer_return);

  ~Function();

  const std::string& GetName() const;

  const std::vector<Variable>& GetArguments() const;

  ReturnType GetReturnType() const;

  bool ReturnsPointer() const;

  bool PassesSema(const Namespace& ns, std::stringstream& stream) const;

  nlohmann::json::object_t GetJSONObject(const Namespace& ns) const;

 private:
  std::string name_;
  std::vector<Variable> arguments_;
  ReturnType return_type_;
  bool pointer_return_ = false;

  std::optional<Primitive> GetPrimitiveReturn() const;

  std::optional<std::string> GetUserDefinedReturn() const;
};

class Struct {
 public:
  Struct();

  Struct(std::string name, std::vector<Variable> variables);

  ~Struct();

  const std::string& GetName() const;

  const std::vector<Variable>& GetVariables() const;

  bool PassesSema(const Namespace& ns, std::stringstream& stream) const;

  nlohmann::json::object_t GetJSONObject(const Namespace& ns) const;

 private:
  std::string name_;
  std::vector<Variable> variables_;
};

class Enum {
 public:
  Enum();

  Enum(std::string name, std::vector<std::string> members);

  ~Enum();

  const std::string& GetName() const;

  const std::vector<std::string>& GetMembers() const;

  bool PassesSema(const Namespace& ns, std::stringstream& stream) const;

  nlohmann::json::object_t GetJSONObject() const;

 private:
  std::string name_;
  std::vector<std::string> members_;
};

using NamespaceItem = std::variant<Function, Struct, Enum>;
using NamespaceItems = std::vector<NamespaceItem>;

class Namespace {
 public:
  Namespace();

  Namespace(std::string name, NamespaceItems items);

  ~Namespace();

  const std::string& GetName() const;

  void SetName(const std::string& name);

  const std::vector<Function>& GetFunctions() const;

  const std::vector<Struct>& GetStructs() const;

  const std::vector<Enum>& GetEnums() const;

  bool HasEnumNamed(const std::string& name) const;

  bool HasStructNamed(const std::string& name) const;

  void AddFunctions(const std::vector<Function>& functions);

  void AddStructs(const std::vector<Struct>& structs);

  void AddEnums(const std::vector<Enum>& enums);

  bool PassesSema(std::stringstream& stream) const;

  nlohmann::json::object_t GetJSONObject() const;

 private:
  std::string name_;
  std::vector<Function> functions_;
  std::vector<Struct> structs_;
  std::vector<Enum> enums_;

  bool CheckDuplicateFunctions(std::stringstream& stream) const;

  bool CheckStructEnumNameCollisions(std::stringstream& stream) const;
};

}  // namespace epoxy
