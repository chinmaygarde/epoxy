#pragma once

#include <string>
#include <variant>
#include <vector>

#include "macros.h"

namespace epoxy {

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
};

class Variable {
 public:
  Variable();

  Variable(Primitive primitive, std::string identifier, bool is_pointer);

  ~Variable();

  Primitive GetPrimitive() const;

  const std::string& GetIdentifier() const;

  bool IsPointer() const;

 private:
  Primitive primitive_;
  std::string identifier_;
  bool is_pointer_ = false;
};

class Function {
 public:
  Function();

  Function(std::string name,
           std::vector<Variable> arguments,
           Primitive return_type,
           bool pointer_return);

  ~Function();

  const std::string& GetName() const;

  const std::vector<Variable>& GetArguments() const;

  Primitive GetReturnType() const;

  bool ReturnsPointer() const;

 private:
  std::string name_;
  std::vector<Variable> arguments_;
  Primitive return_type_;
  bool pointer_return_ = false;
};

class Struct {
 public:
  Struct();

  Struct(std::string name, std::vector<Variable> variables);

  ~Struct();

  const std::string& GetName() const;

  const std::vector<Variable>& GetVariables() const;

 private:
  std::string name_;
  std::vector<Variable> variables_;
};

using NamespaceItem = std::variant<Function, Struct>;
using NamespaceItems = std::vector<NamespaceItem>;

class Namespace {
 public:
  Namespace();

  Namespace(std::string name, NamespaceItems items);

  ~Namespace();

  const std::string& GetName() const;

  const std::vector<Function>& GetFunctions() const;

  const std::vector<Struct>& GetStructs() const;

 private:
  std::string name_;
  std::vector<Function> functions_;
  std::vector<Struct> structs_;
};

}  // namespace epoxy
