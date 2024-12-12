#include <iostream>
#include <fstream>
#include <exception>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Ast.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  std::ifstream in(argv[1]);

  if (!in.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  Lexer lexer(in);
  Parser parser(lexer);

  try
  {
    AstNode *prg = parser.parse();
    std::cout << "Sin Errores!" << std::endl;
    std::cout << prg->toString() << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
