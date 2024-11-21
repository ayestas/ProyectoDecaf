#include "Parser.hpp"
#include <string>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

void Parser::parse()
{
  programa();
}

void Parser::programa()
{
  ErrorEval(Token::KW_CLASS, "Expected kw_class");
  currentToken = lexer.nextToken();

  std::cout << "Token: " << lexer.tokenText() << std::endl;
  
  ErrorEval(Token::OPEN_CURLY, "Expected '{'");
  currentToken = lexer.nextToken();
  
  std::cout << "Token: " << lexer.tokenText() << std::endl;
  ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
}

void Parser::ErrorEval(Token expectedToken, const std::string &message)
{
  if (currentToken != expectedToken)
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": " + message);
  }
}
