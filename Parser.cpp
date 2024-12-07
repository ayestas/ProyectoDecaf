#include "Parser.hpp"
#include <string>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

void Parser::parse()
{
  programa();
}

void Parser::programa()
{
  while (currentToken != Token::Eof)
  {
    if (currentToken == Token::Eol)
    {
      currentToken = lexer.nextToken();
      continue;
    }

    class_declaration();
  }
};

void Parser::class_declaration()
{
  if (currentToken == Token::KW_CLASS)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::IDENT, "Expected identifier");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    funcion();

    if (currentToken == Token::KW_INT)
    {
      while (currentToken == Token::KW_INT)
      {
        funcion();
      }
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected 'class'");
  }
}

void Parser::funcion()
{
  if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::IDENT, "Expected IDENT");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_PAR, "Expected '('");
    currentToken = lexer.nextToken();

    param_list();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::KW_INT)
    {
      var_decl();
      ErrorEval(Token::SEMICOLON, "Expected ';'");
      currentToken = lexer.nextToken();
    }

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      declaraciones();
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected KW_INT");
  }
}

void Parser::param_list()
{
  if (currentToken == Token::KW_INT)
  {
    param();
    while (currentToken == Token::COMMA)
    {
      currentToken = lexer.nextToken();
      param();
    }
  }
  else if (currentToken == Token::KW_REF)
  {
    currentToken = lexer.nextToken();
    param();
    while (currentToken == Token::COMMA)
    {
      currentToken = lexer.nextToken();
      param();
    }
  }
}

void Parser::param()
{
  if (currentToken == Token::KW_REF)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::KW_INT, "Expected KW_INT");
    currentToken = lexer.nextToken();
  }
  else if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();
  }

  ErrorEval(Token::IDENT, "Expected IDENT");
  currentToken = lexer.nextToken();
  if (currentToken == Token::OPEN_BRACKET)
  {
    currentToken = lexer.nextToken();
    ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
    currentToken = lexer.nextToken();
  }
}

void Parser::var_decl()
{
  if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::IDENT, "Expected IDENT");
    currentToken = lexer.nextToken();

    if (currentToken == Token::OPEN_BRACKET)
    {
      array();

      if (currentToken == Token::COMMA)
      {
        while (currentToken == Token::COMMA)
        {
          currentToken = lexer.nextToken();

          ErrorEval(Token::IDENT, "Expected IDENT");
          currentToken = lexer.nextToken();

          if (currentToken == Token::OPEN_BRACKET)
          {
            array();
          }
        }
      }
    }
    else if (currentToken == Token::COMMA)
    {
      while (currentToken == Token::COMMA)
      {
        currentToken = lexer.nextToken();

        ErrorEval(Token::IDENT, "Expected IDENT");
        currentToken = lexer.nextToken();

        if (currentToken == Token::OPEN_BRACKET)
        {
          array();
        }
      }
    }
  }
}

void Parser::array()
{
  currentToken = lexer.nextToken();

  if (currentToken == Token::NUMBER || currentToken == Token::IDENT)
  {
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected NUMBER or IDENT");
  }

  ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
  currentToken = lexer.nextToken();
}

void Parser::declaraciones()
{
  switch (currentToken)
  {
  case Token::IDENT:
    currentToken = lexer.nextToken();
    if (currentToken == Token::OPEN_PAR)
    {
      currentToken = lexer.nextToken();
      expr_list();
      ErrorEval(Token::CLOSE_PAR, "Expected ')'");
      currentToken = lexer.nextToken();
    }
    else if (currentToken == Token::OPEN_BRACKET)
    {
      currentToken = lexer.nextToken();
      expr();
      ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
      currentToken = lexer.nextToken();
      decl_assign();
    }
    else
    {
      decl_assign();
    }
    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();
    break;
  case Token::KW_IF:
    decl_if();
    break;
  case Token::KW_WHILE:
    decl_while();
    break;
  case Token::KW_FOR:
    decl_for();
    break;
  case Token::KW_SOUT:
    decl_print();
    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();
    break;
  case Token::KW_SOUTLN:
    decl_print();
    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();
    break;

  default:
    break;
  }
}

void Parser::decl_assign()
{
  ErrorEval(Token::ASSIGN, "Expected '='");
  currentToken = lexer.nextToken();
  expr();
}

void Parser::decl_if()
{
  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();
    expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      declaraciones();
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();

    if (currentToken == Token::KW_ELSE)
    {
      currentToken = lexer.nextToken();
      ErrorEval(Token::OPEN_CURLY, "Expected '{'");

      currentToken = lexer.nextToken();
      while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
      {
        declaraciones();
      }

      ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
      currentToken = lexer.nextToken();
    }
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }
}

void Parser::decl_while()
{
  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();
    expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      declaraciones();
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }
}

void Parser::decl_for()
{
  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::KW_INT)
    {
      currentToken = lexer.nextToken();

      ErrorEval(Token::IDENT, "Expected IDENT");
      currentToken = lexer.nextToken();
    }
    else if (currentToken == Token::IDENT)
    {
      currentToken = lexer.nextToken();
    }

    ErrorEval(Token::ASSIGN, "Expected '='");
    currentToken = lexer.nextToken();

    expr();

    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();

    expr();

    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::IDENT, "Expected IDENT");
    currentToken = lexer.nextToken();

    ErrorEval(Token::ASSIGN, "Expected '='");
    currentToken = lexer.nextToken();

    expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      declaraciones();
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }
}

void Parser::decl_print()
{
  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::IDENT)
    {
      currentToken = lexer.nextToken();

      if (currentToken == Token::OPEN_BRACKET)
      {
        array();
      }
    }
    else
    {
      ErrorEval(Token::STRING_LITERAL, "Expected STRING");
      currentToken = lexer.nextToken();
    }

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }
}

void Parser::expr()
{
  bool_term();
  while (currentToken == Token::BOOL_OR)
  {
    currentToken = lexer.nextToken();
    bool_term();
  }
}

void Parser::bool_term()
{
  rel_expr();
  while (currentToken == Token::BOOL_AND)
  {
    currentToken = lexer.nextToken();
    rel_expr();
  }
}

void Parser::rel_expr()
{
  arith_expr();
  if (currentToken == Token::GT || currentToken == Token::LT || currentToken == Token::GET || currentToken == Token::LET || currentToken == Token::NE || currentToken == Token::EQ)
  {
    currentToken = lexer.nextToken();
    arith_expr();
  }
}

void Parser::arith_expr()
{
  arith_term();
  while (currentToken == Token::OP_ADD || currentToken == Token::OP_SUB)
  {
    currentToken = lexer.nextToken();
    arith_term();
  }
}

void Parser::arith_term()
{
  arith_factor();
  while (currentToken == Token::OP_MUL || currentToken == Token::OP_DIV || currentToken == Token::OP_MOD)
  {
    currentToken = lexer.nextToken();
    arith_factor();
  }
}

void Parser::arith_factor()
{
  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();
    expr();
    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();
  }
  else if (currentToken == Token::IDENT)
  {
    currentToken = lexer.nextToken();
    if (currentToken == Token::OPEN_BRACKET)
    {
      currentToken = lexer.nextToken();
      expr();
      ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
      currentToken = lexer.nextToken();
    }
    else if (currentToken == Token::OPEN_PAR)
    {
      currentToken = lexer.nextToken();
      if (currentToken == Token::CLOSE_PAR)
      {
        currentToken = lexer.nextToken();
      }
      else
      {
        expr_list();
        currentToken = lexer.nextToken();
      }
    }
  }
  else if (currentToken == Token::KW_READ)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_PAR, "Expected '('");
    currentToken = lexer.nextToken();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();
  }
  else if (currentToken == Token::NUMBER)
  {
    currentToken = lexer.nextToken();
  }
  else if (currentToken == Token::CLOSE_PAR)
  {
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected IDENT, INT_LITERAL or '('");
  }
}

void Parser::expr_list()
{
  expr();
  while (currentToken == Token::COMMA)
  {
    currentToken = lexer.nextToken();
    expr();
  }
}

// Detecta si el token esperado es igual al token actual, si no lo es, lanza una excepción
void Parser::ErrorEval(Token expectedToken, const std::string &message)
{
  if (currentToken != expectedToken)
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": " + message);
  }
}
