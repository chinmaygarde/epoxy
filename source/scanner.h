// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include "decls.h"
#include "lexer.h"
#include "macros.h"

namespace epoxy {

class Scanner {
 public:
  Scanner(const std::string& text);

  ~Scanner();

  bool IsValid() const;

  yyscan_t GetHandle();

 private:
  yyscan_t scanner_ = {};
  YY_BUFFER_STATE buffer_ = {};
  bool is_valid_ = false;

  EPOXY_DISALLOW_COPY_AND_ASSIGN(Scanner);
};

}  // namespace epoxy
