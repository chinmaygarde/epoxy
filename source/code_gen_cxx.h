#pragma once

#include "code_gen.h"
#include "macros.h"

namespace epoxy {

class CodeGenCXX final : public CodeGen {
 public:
  CodeGenCXX();

  // |CodeGen|
  ~CodeGenCXX() override;

  // |CodeGen|
  RenderResult Render(std::vector<Namespace> namespaces) const override;

 private:
  EPOXY_DISALLOW_COPY_AND_ASSIGN(CodeGenCXX);
};

}  // namespace epoxy
