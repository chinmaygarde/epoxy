#pragma once

#include "code_gen.h"
#include "macros.h"

namespace epoxy {

class CodeGenDart final : public CodeGen {
 public:
  CodeGenDart();

  // |CodeGen|
  ~CodeGenDart() override;

  // |CodeGen|
  RenderResult Render(std::vector<Namespace> namespaces) const override;

 private:
  EPOXY_DISALLOW_COPY_AND_ASSIGN(CodeGenDart);
};

}  // namespace epoxy
