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

 private:
  std::string name_;
};

}  // namespace epoxy
