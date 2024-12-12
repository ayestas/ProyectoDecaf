#pragma once
#include "Lexer.hpp"
#include "Ast.hpp"
#include <stdexcept>
#include <fstream>

class Parser
{
public:
  Parser(Lexer &lex);

  AstNode *parse();

  AstNode *programa();
  AstNode *funcion();
  AstNode *param_list();
  AstNode *param();
  AstNode *var_decl();
  AstNode *array();
  AstNode *declaraciones();

  AstNode *decl_assign();
  AstNode *decl_if();
  AstNode *decl_while();
  AstNode *decl_for();
  AstNode *decl_print(bool line);

  AstNode *expr();
  AstNode *bool_term();
  AstNode *rel_expr();
  AstNode *arith_expr();
  AstNode *arith_term();
  AstNode *arith_factor();
  AstNode *expr_list();

  AstNode *class_declaration();
  
  std::string currentTokenText() const
  {
    return lexer.tokenText();
  }

private:
  Lexer &lexer;
  Token currentToken;
  void ErrorEval(Token expectedToken, const std::string &message);
};
