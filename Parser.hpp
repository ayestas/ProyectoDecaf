#pragma once
#include "Lexer.hpp"
#include <stdexcept>
#include <fstream>

class Parser
{
public:
  Parser(Lexer &lex);

  void parse();

  void programa();
  void funcion();
  void param_list();
  void param();
  void var_decl();
  void array();
  void declaraciones();

  void decl_assign();
  void decl_if();
  void decl_while();
  void decl_for();
  void decl_print();

  void expr();
  void bool_term();
  void rel_expr();
  void arith_expr();
  void arith_term();
  void arith_factor();
  void expr_list();

  void class_declaration();
  
  std::string currentTokenText() const
  {
    return lexer.tokenText();
  }

private:
  Lexer &lexer;
  Token currentToken;
  void ErrorEval(Token expectedToken, const std::string &message);
};
