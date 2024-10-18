#include <cstring>
#include "Lexer.hpp"

/*!max:re2c*/
Lexer::Lexer(std::istream &in)
    : inbuff(in), line(1) {}

int Lexer::errLine() const
{
  return line;
}

Lexer::InputBuffer::InputBuffer(std::istream &in)
    : in(in),
      buf(new char[SIZE + YYMAXFILL]),
      lim(buf + SIZE),
      cur(lim),
      tok(lim),
      eof(false)
{
}

bool Lexer::InputBuffer::fill(size_t need)
{
  if (eof)
    return false;

  const size_t free = tok - buf;
  if (free < need)
    return false;

  memmove(buf, tok, lim - tok);
  lim -= free;
  cur -= free;
  tok -= free;

  in.read(lim, free);
  lim += in.gcount();

  if (lim < buf + SIZE)
  {
    eof = true;
    memset(lim, 0, YYMAXFILL);
    lim += YYMAXFILL;
  }
  return true;
}

const std::string kw_if = "if";
const std::string kw_else = "else";
const std::string kw_while = "while";
const std::string kw_for = "for";
const std::string kw_int = "int";
const std::string kw_sout = "System.out.print";
const std::string kw_soutln = "System.out.println";

Token Lexer::nextToken()
{
  while (true)
  {
    inbuff.tok = inbuff.cur;
    /*!re2c
      re2c:define:YYCTYPE = char;
      re2c:define:YYCURSOR = inbuff.cur;
      re2c:define:YYLIMIT = inbuff.lim;
      re2c:define:YYMARKER = inbuff.mark;
      re2c:define:YYFILL = "if (!inbuff.fill(@@)) return (inbuff.eof)? Token::Eof : Token::Error;";
      re2c:define:YYFILL:naked = 1;

      end = "\x00";
      wsp = [ \t]+;
      eol = "\n";

      // Comentarios
      comment_block = "/*" [^*]* "*" ("*" [^/]* "/*" [^*]* "*")* "/";
      comment_line = "//" [^\n]*;

      //Datos
      number = [0-9]+("." [0-9]+)?;
      ident = [a-zA-Z_][a-zA-Z0-9_]*;
      string = "\"" [^\"]* "\"";

      // Keywords
      kw_if = "if";
      kw_else = "else";
      kw_while = "while";
      kw_for = "for";
      kw_int = "int";

      // Símbolos
      comma = ",";
      semicolon = ";";
      ampersand = "&";
      open_par = "(";
      close_par = ")";
      open_bracket = "[";
      close_bracket = "]";
      open_curly = "{";
      close_curly = "}";
      op_assign = "=";
      increment = "++";
      decrement = "--";

      // Operadores de comparación
      gt = ">";
      lt = "<";
      get = ">=";
      let = "<=";
      eq = "==";
      ne = "!=";
      bool_or = "||";
      bool_and = "&&";

      // Operadores Aritméticos
      op_add = "+";
      op_sub = "-";
      op_mul = "*";
      op_div = "/";
      op_mod = "%";

      // Datos
      number { text = inbuff.tokenText(); return Token::NUMBER; }
      ident {
        text = inbuff.tokenText();
        if (text == kw_if) return Token::KW_IF;
        if (text == kw_else) return Token::KW_ELSE;
        if (text == kw_while) return Token::KW_WHILE;
        if (text == kw_for) return Token::KW_FOR;
        if (text == kw_int) return Token::KW_INT;
        if (text == kw_sout) return Token::KW_SOUT;
        if (text == kw_soutln) return Token::KW_SOUTLN;
        return Token::IDENT;
      }
      string { text = inbuff.tokenText(); return Token::STRING_LITERAL; }
      
      comment_block | comment_line { continue; }

      // Símbolos
      comma           { return Token::COMMA; }
      semicolon       { return Token::SEMICOLON; }
      ampersand       { return Token::AMPERSAND; }
      open_par        { return Token::OPEN_PAR; }
      close_par       { return Token::CLOSE_PAR; }
      open_bracket    { return Token::OPEN_BRACKET; }
      close_bracket   { return Token::CLOSE_BRACKET; }
      open_curly      { return Token::OPEN_CURLY; }
      close_curly     { return Token::CLOSE_CURLY; }
      op_assign       { return Token::OP_ASSIGN; }
      increment       { return Token::INCREMENT; }
      decrement       { return Token::DECREMENT; }

      // Operadores Lógicos
      gt              { return Token::GT; }
      lt              { return Token::LT; }
      get             { return Token::GET; }
      let             { return Token::LET; }
      eq              { return Token::EQ; }
      ne              { return Token::NE; }
      bool_or         { return Token::BOOL_OR; }
      bool_and        { return Token::BOOL_AND; }

      // Operadores Aritméticos
      op_add          { return Token::OP_ADD; }
      op_sub          { return Token::OP_SUB; }
      op_mul          { return Token::OP_MUL; }
      op_div          { return Token::OP_DIV; }
      op_mod          { return Token::OP_MOD; }

      end             { return (YYMAXFILL == inbuff.lim - inbuff.tok)? Token::Eof : Token::Error; }
      wsp             { continue; }
      eol             { ++line; continue; }
    */
  }  
}
