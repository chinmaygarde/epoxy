#pragma once

#include <string>
#include <variant>
#include <vector>

#include "macros.h"

namespace epoxy {

enum class Primitive {
  kInt8,
  kInt16,
  kInt32,
  kInt64,
  kUnsignedInt8,
  kUnsignedInt16,
  kUnsignedInt32,
  kUnsignedInt64,
};

class Argument {
 public:
  Argument();

  Argument(Primitive primitive, std::string identifier);

  ~Argument();

  Primitive GetPrimitive() const;

  const std::string& GetIdentifier() const;

 private:
  Primitive primitive_;
  std::string identifier_;
};

class Function {
 public:
  Function();

  Function(std::string name,
           std::vector<Argument> arguments,
           Primitive return_type);

  ~Function();

  const std::string& GetName() const;

  const std::vector<Argument>& GetArguments() const;

  Primitive GetReturnType() const;

 private:
  std::string name_;
  std::vector<Argument> arguments_;
  Primitive return_type_;
};

class Struct {
 public:
  Struct();

  Struct(std::string name, std::vector<Argument> variables);

  ~Struct();

  const std::string& GetName() const;

  const std::vector<Argument> GetVariables() const;

 private:
  std::string name_;
  std::vector<Argument> variables_;
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
