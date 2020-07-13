#pragma once

#include <optional>
#include <string>
#include <vector>

#include "macros.h"
#include "types.h"

namespace epoxy {

class CodeGen {
 public:
  enum class Type {
    kCXX,
    kDart,
  };
  static std::unique_ptr<CodeGen> CreateGenerator(Type type);

  CodeGen(std::string template_data);

  virtual ~CodeGen();

  struct RenderResult {
    std::optional<std::string> result;
    std::optional<std::string> error;
  };

  RenderResult Render(std::vector<Namespace> namespaces) const;

 private:
  std::string template_data_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace epoxy
