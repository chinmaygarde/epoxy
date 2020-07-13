// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "scanner.h"

namespace epoxy {

Scanner::Scanner(const std::string& text)
    : scanner_(nullptr), buffer_(nullptr), is_valid_(false) {
  if (epoxy_lex_init(&scanner_) != 0) {
    return;
  }
  buffer_ = epoxy__scan_string(text.data(), scanner_);
  is_valid_ = true;
}

bool Scanner::IsValid() const {
  return is_valid_;
}

yyscan_t Scanner::GetHandle() {
  return scanner_;
}

Scanner::~Scanner() {
  if (!is_valid_) {
    return;
  }

  epoxy__delete_buffer(buffer_, scanner_);
  epoxy_lex_destroy(scanner_);
}

}  // namespace epoxy
