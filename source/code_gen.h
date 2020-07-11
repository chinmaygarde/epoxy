#pragma once

#include <optional>
#include <string>
#include <vector>

#include "macros.h"
#include "types.h"

namespace epoxy {

class CodeGen {
 public:
  CodeGen();

  virtual ~CodeGen();

  struct RenderResult {
    std::optional<std::string> result;
    std::optional<std::string> error;
  };

  virtual RenderResult Render(std::vector<Namespace> namespaces) const = 0;

 private:
  EPOXY_DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace epoxy
