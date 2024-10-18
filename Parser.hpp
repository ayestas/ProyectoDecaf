#include "Lexer.hpp"
#include <stdexcept>
#include <fstream>

class Parser
{
public:
  Parser(Lexer &lex);

  void parse();

private:
  Lexer &lexer;
  Token currentToken;
  void ErrorEval(Token expectedToken, const std::string &message);
};
