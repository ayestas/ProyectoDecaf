#pragma once

#include <string>
#include <fstream>
#include <iostream>

enum Token
{
  Eof,
  Error,
  // Keywords
  KW_IF,
  KW_CLASS,
  KW_ELSE,
  KW_WHILE,
  KW_FOR,
  KW_INT,
  KW_SOUT,
  KW_SOUTLN,
  // Símbolos
  COMMA,
  SEMICOLON,
  AMPERSAND,
  OPEN_PAR,
  CLOSE_PAR,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  OPEN_CURLY,
  CLOSE_CURLY,
  OP_ASSIGN,
  INCREMENT,
  DECREMENT,
  // Operadores Lógicos
  GT,
  LT,
  GET,
  LET,
  EQ,
  NE,
  BOOL_OR,
  BOOL_AND,
  // Operadores Aritméticos
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  // Datos
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
