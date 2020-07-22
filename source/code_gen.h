// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "macros.h"
#include "types.h"

namespace epoxy {

class CodeGen {
 public:
  CodeGen(std::string template_data);

  virtual ~CodeGen();

  struct RenderResult {
    std::optional<std::string> result;
    std::optional<std::string> error;
  };

  std::string GenerateTemplateDataJSON(
      const std::vector<Namespace>& namespaces) const;

  RenderResult Render(const std::vector<Namespace>& namespaces) const;

 private:
  std::string template_data_;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace epoxy
