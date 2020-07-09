#pragma once

#include <string>
#include <variant>
#include <vector>

#include "macros.h"

namespace epoxy {

enum class Primitive {
  kInt32,
  kInt64,
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

using NamespaceItem = std::variant<Function>;
using NamespaceItems = std::vector<NamespaceItem>;

class Namespace {
 public:
  Namespace();

  Namespace(std::string name, NamespaceItems items);

  ~Namespace();

  const std::string& GetName() const;

  const std::vector<Function>& GetFunctions() const;

 private:
  std::string name_;
  std::vector<Function> functions_;
};

}  // namespace epoxy
