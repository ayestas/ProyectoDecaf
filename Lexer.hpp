#pragma once

#include <string>
#include <fstream>
#include <iostream>

enum Token
{
  Eof,
  Error,
  Eol,
  // KEYWORDS
  KW_CLASS,
  KW_INT,
  KW_REF,
  KW_IF,
  KW_ELSE,
  KW_WHILE,
  KW_FOR,
  KW_READ,
  KW_SOUT,
  KW_SOUTLN,
  //SIMBOLOS
  COMMA,
  SEMICOLON,
  AMPERSAND,
  OPEN_PAR,
  CLOSE_PAR,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  OPEN_CURLY,
  CLOSE_CURLY,
  ASSIGN,
  // OPERADORES DE COMPARACIÓN
  GT,
  LT,
  GET,
  LET,
  EQ,
  NE,
  BOOL_OR,
  BOOL_AND,
  // OPERADORES ARITMÉTICOS
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  // DATOS
  IDENT,
  NUMBER,
  STRING_LITERAL
};

class Lexer
{
public:
  static const size_t SIZE = 512;

  Lexer(std::istream &in);

  Token nextToken();

  std::string tokenText() const
  {
    return text;
  }

  int errLine() const;

private:
  struct InputBuffer
  {
    char *buf;
    char *lim; // Limite del buffer
    char *cur; // Cursor
    char *mark;
    char *tok; // Apuntador al inicio del lexema

    bool eof;
    std::istream &in;

    InputBuffer(std::istream &in);

    std::string tokenText() const
    {
      return std::string(tok, cur);
    }

    ~InputBuffer()
    {
      delete[] buf;
    }

    bool fill(size_t need);
  };

private:
  InputBuffer inbuff;
  std::string text;
  int line;
};
