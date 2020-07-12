#include "code_gen.h"

#include "code_gen_cxx.h"
#include "code_gen_dart.h"

namespace epoxy {

std::unique_ptr<CodeGen> CodeGen::CreateGenerator(Type type) {
  switch (type) {
    case Type::kCXX:
      return std::make_unique<CodeGenCXX>();
    case Type::kDart:
      return std::make_unique<CodeGenDart>();
  }
  return nullptr;
}

CodeGen::CodeGen() = default;

CodeGen::~CodeGen() = default;

}  // namespace epoxy
