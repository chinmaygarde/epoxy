#pragma once

#include "driver.h"
#include "parser.h"

#define YY_DECL \
  epoxy::Parser::symbol_type epoxy_lex(epoxy::Driver& driver, void* yyscanner)

YY_DECL;
