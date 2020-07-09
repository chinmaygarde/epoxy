%skeleton "lalr1.cc"
%require "3.0.4"

%defines "parser.h"
%output "parser.cc"

%define api.parser.class {Parser}
%define api.prefix {epoxy_}
%define api.namespace {epoxy}
%define api.token.constructor
%define api.value.type variant
%define parse.trace
%define parse.error verbose
%define api.token.prefix {epoxy_TOKEN_}
%define parse.assert

%param { epoxy::Driver& driver }
%param { void *scanner }

%no-lines
%locations

%initial-action {

};

%code requires {
#include "driver.h"
#include "types.h"
}

%code {
#include "decls.h"
#include "lexer.h"
}

%token
  END 0                   "<end of contents>"

  NAMESPACE               "namespace"
  CLASS                   "class"
  STRUCT                  "struct"
  FUNCTION                "function"

  INVALID_TOKEN           "<invalid token>"

  SEMI_COLON              ";"
  CURLY_LEFT              "{"
  CURLY_RIGHT             "}"
  PAREN_LEFT              "("
  PAREN_RIGHT             ")"
  COMMA                   ","

  INT_32_T                "int32_t"
  INT_64_T                "int64_t"

  ARROW                   "->"
  ;

%token <std::string>
  IDENTIFIER      "<identifier>"

%type <epoxy::Namespace> Namespace
%type <epoxy::NamespaceItems> NamespaceItems
%type <epoxy::NamespaceItem> NamespaceItem
%type <epoxy::Function> Function
%type <epoxy::Argument> Argument
%type <std::vector<epoxy::Argument>> ArgumentList
%type <std::vector<epoxy::Argument>> VariableList
%type <epoxy::Primitive> Primitive
%type <epoxy::Struct> Struct

%start SourceFile

%%

SourceFile
  : Namespace { driver.AddNamespace($1); }
    | %empty
  ;

Namespace
  : NAMESPACE IDENTIFIER CURLY_LEFT NamespaceItems CURLY_RIGHT { $$ = epoxy::Namespace{$2, $4}; }
  | NAMESPACE IDENTIFIER CURLY_LEFT                CURLY_RIGHT { $$ = epoxy::Namespace{$2, {}}; }
  ;

NamespaceItems
  : NamespaceItem                  { $$ = {$1}; }
  | NamespaceItems NamespaceItem   { $$ = $1; $$.push_back($2); }
  ;

NamespaceItem
  : Function  { $$ = $1; }
  | Struct    { $$ = $1; }
  ;

Function
  : FUNCTION IDENTIFIER PAREN_LEFT ArgumentList PAREN_RIGHT ARROW Primitive { $$ = epoxy::Function{$2, $4, $7}; }
  | FUNCTION IDENTIFIER PAREN_LEFT              PAREN_RIGHT ARROW Primitive { $$ = epoxy::Function{$2, {}, $6}; }
  ;

ArgumentList
  : Argument                        { $$ = {$1}; }
  | ArgumentList COMMA Argument     { $$ = $1; $$.push_back($3); }
  ;

Struct
  : STRUCT IDENTIFIER CURLY_LEFT VariableList  CURLY_RIGHT { $$ = epoxy::Struct{$2, $4}; }
  | STRUCT IDENTIFIER CURLY_LEFT               CURLY_RIGHT { $$ = epoxy::Struct{$2, {}}; }
  ;

VariableList
  : Argument     SEMI_COLON     { $$ = {$1}; }
  | VariableList Argument       { $$ = $1; $$.push_back($2); }
  ;

Argument
  : Primitive IDENTIFIER { $$ = epoxy::Argument{$1, $2}; }
  ;

Primitive
  : INT_32_T { $$ = epoxy::Primitive::kInt32; }
  | INT_64_T { $$ = epoxy::Primitive::kInt64; }
  ;

%%

void epoxy::Parser::error(const epoxy::Parser::location_type& loc,
                          const std::string& message) {
  driver.ReportParsingError(loc, message);
}
