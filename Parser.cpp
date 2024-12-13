#include "Parser.hpp"
#include <string>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

AstNode *Parser::parse()
{
  AstNode *prog = programa();

  return prog;
}

AstNode *Parser::programa()
{
  AstNodeList clases;

  while (currentToken != Token::Eof)
  {
    if (currentToken == Token::Eol)
    {
      currentToken = lexer.nextToken();
      continue;
    }

    clases.push_back(class_declaration());
  }

  return new ProgramaNodo(clases);
};

AstNode *Parser::class_declaration()
{
  AstNodeList funciones;
  std::string ident = "";

  if (currentToken == Token::KW_CLASS)
  {
    currentToken = lexer.nextToken();

    ident = currentTokenText();

    ErrorEval(Token::IDENT, "Expected identifier");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    funciones.push_back(funcion());

    if (currentToken == Token::KW_INT)
    {
      while (currentToken == Token::KW_INT)
      {
        funciones.push_back(funcion());
      }
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected 'class'");
  }

  return new ClaseDeclNodo(new IdentNode(ident, false, false, false, false, defaultNode), funciones);
}

AstNode *Parser::funcion()
{
  std::string ident = "";
  AstNode *paramList = defaultNode;
  AstNodeList varDecls;
  AstNodeList decls;

  if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();

    ident = currentTokenText();

    ErrorEval(Token::IDENT, "Expected IDENT");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_PAR, "Expected '('");
    currentToken = lexer.nextToken();

    paramList = param_list();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::KW_INT)
    {
      varDecls.push_back(var_decl());
      ErrorEval(Token::SEMICOLON, "Expected ';'");
      currentToken = lexer.nextToken();
    }

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      decls.push_back(declaraciones());
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected KW_INT");
  }

  return new FuncionNodo(new IdentNode(ident, false, false, false, false, defaultNode), paramList, varDecls, decls);
}

AstNode *Parser::param_list()
{
  AstNodeList params;
  bool isRef = false;

  if (currentToken == Token::KW_INT)
  {
    params.push_back(param());
    while (currentToken == Token::COMMA)
    {
      currentToken = lexer.nextToken();
      params.push_back(param());
    }
  }
  else if (currentToken == Token::KW_REF)
  {
    isRef = true;
    currentToken = lexer.nextToken();
    params.push_back(param());
    while (currentToken == Token::COMMA)
    {
      currentToken = lexer.nextToken();
      params.push_back(param());
    }
  }

  return new ParamListNodo(params, isRef);
}

AstNode *Parser::param()
{
  bool isRef = false;
  bool isArray = false;
  AstNode *ident = defaultNode;

  if (currentToken == Token::KW_REF)
  {
    isRef = true;
    currentToken = lexer.nextToken();

    ErrorEval(Token::KW_INT, "Expected KW_INT");
    currentToken = lexer.nextToken();
  }
  else if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();
  }

  ident = variable();

  return new ParamNodo(isRef, ident);
}

AstNode *Parser::var_decl()
{
  AstNodeList identList;

  if (currentToken == Token::KW_INT)
  {
    currentToken = lexer.nextToken();
    identList.push_back(variable());

    if (currentToken == Token::COMMA)
    {
      while (currentToken == Token::COMMA)
      {
        currentToken = lexer.nextToken();
        identList.push_back(variable());
      }
    }
  }
  else
  {
    throw std::runtime_error("Expected type declaration");
  }

  return new VariableDeclNodo(identList);
}

AstNode *Parser::variable()
{
  std::string ident = currentTokenText();
  bool isArray = false;
  bool isEmptyArray = false;
  bool isFuncCall = false;
  bool isEmptyFuncCall = false;
  AstNode *e = defaultNode;

  currentToken = lexer.nextToken();
  if (currentToken == Token::OPEN_BRACKET)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::CLOSE_BRACKET)
    {
      isEmptyArray = true;
      currentToken = lexer.nextToken();
    }
    else
    {
      isArray = true;
      e = expr();

      ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
      currentToken = lexer.nextToken();
    }
  }
  else if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::CLOSE_PAR)
    {
      isEmptyFuncCall = true;
      currentToken = lexer.nextToken();
    }
    else
    {
      isFuncCall = true;
      e = expr_list();
      currentToken = lexer.nextToken();
    }
  }

  return new IdentNode(ident, isArray, isEmptyArray, isFuncCall, isEmptyFuncCall, e);
}

AstNode *Parser::array()
{
  std::string numero = "";
  AstNode *constInt = defaultNode;

  currentToken = lexer.nextToken();

  if (currentToken == Token::NUMBER || currentToken == Token::IDENT)
  {
    numero = currentTokenText();
    constInt = new ConstIntNode(numero);
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected NUMBER or IDENT");
  }

  ErrorEval(Token::CLOSE_BRACKET, "Expected ']'");
  currentToken = lexer.nextToken();

  return new ArrayNodo(constInt);
}

AstNode *Parser::declaraciones()
{
  AstNode *decl = defaultNode;
  switch (currentToken)
  {
  case Token::IDENT:
    decl = decl_assign();
    break;
  case Token::KW_IF:
    decl = decl_if();
    break;
  case Token::KW_WHILE:
    decl = decl_while();
    break;
  case Token::KW_FOR:
    decl = decl_for();
    break;
  case Token::KW_SOUT:
    decl = decl_print(false);
    break;
  case Token::KW_SOUTLN:
    decl = decl_print(true);
    break;
  default:
    throw std::runtime_error("Unexpected token in declaraciones");
  }
  if (!decl)
  {
    throw std::runtime_error("Failed to create declaration node");
  }
  return new Declaraciones(decl);
}

AstNode *Parser::decl_assign()
{
  AstNode *ident = variable();
  AstNode *e = defaultNode;
  bool isAssign = false;

  if (currentToken == Token::ASSIGN)
  {
    isAssign = true;
    currentToken = lexer.nextToken();
    e = expr();

    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();
  }
  else
  {
    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();
  }

  return new AsignarDecl(ident, e, isAssign);
}

AstNode *Parser::decl_if()
{
  AstNode *condicion = defaultNode;
  AstNodeList ifDecls;
  AstNodeList elseDecls;

  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();
    condicion = expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      ifDecls.push_back(declaraciones());
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
        elseDecls.push_back(declaraciones());
      }

      ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
      currentToken = lexer.nextToken();
    }
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }

  return new IfElseDecl(condicion, ifDecls, elseDecls);
}

AstNode *Parser::decl_while()
{
  AstNode *condicion = defaultNode;
  AstNodeList decls;

  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();
    condicion = expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      decls.push_back(declaraciones());
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }

  return new WhileDecl(condicion, decls);
}

AstNode *Parser::decl_for()
{
  std::string ident = "";
  AstNode *condExpr1 = defaultNode;
  AstNode *condExpr2 = defaultNode;
  AstNode *condExpr3 = defaultNode;
  bool hasInt = false;
  AstNodeList decls;

  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::KW_INT)
    {
      hasInt = true;
      currentToken = lexer.nextToken();

      ident = currentTokenText();

      ErrorEval(Token::IDENT, "Expected IDENT");
      currentToken = lexer.nextToken();
    }
    else if (currentToken == Token::IDENT)
    {
      ident = currentTokenText();
      currentToken = lexer.nextToken();
    }

    ErrorEval(Token::ASSIGN, "Expected '='");
    currentToken = lexer.nextToken();

    condExpr1 = expr();

    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();

    condExpr2 = expr();

    ErrorEval(Token::SEMICOLON, "Expected ';'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::IDENT, "Expected IDENT");
    currentToken = lexer.nextToken();

    ErrorEval(Token::ASSIGN, "Expected '='");
    currentToken = lexer.nextToken();

    condExpr3 = expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_CURLY, "Expected '{'");
    currentToken = lexer.nextToken();

    while (currentToken == Token::IDENT || currentToken == Token::KW_IF || currentToken == Token::KW_WHILE || currentToken == Token::KW_FOR || currentToken == Token::KW_READ || currentToken == Token::KW_SOUT || currentToken == Token::KW_SOUTLN)
    {
      decls.push_back(declaraciones());
    }

    ErrorEval(Token::CLOSE_CURLY, "Expected '}'");
    currentToken = lexer.nextToken();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected OPEN_PAR");
  }

  return new ForDecl(new IdentNode(ident, false, false, false, false, defaultNode), condExpr1, condExpr2, condExpr3, hasInt, decls);
}

AstNode *Parser::decl_print(bool line)
{
  AstNode *e = defaultNode;

  currentToken = lexer.nextToken();

  if (currentToken == Token::OPEN_PAR)
  {
    currentToken = lexer.nextToken();

    if (currentToken == Token::IDENT)
    {
      e = variable();
    }
    else
    {
      e = new StringLitNode(currentTokenText());
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
  ErrorEval(Token::SEMICOLON, "Expected ';'");
  currentToken = lexer.nextToken();

  return new PrintDecl(e, line);
}

AstNode *Parser::expr()
{
  AstNodeList boolTerms;

  boolTerms.push_back(bool_term());
  while (currentToken == Token::BOOL_OR)
  {
    currentToken = lexer.nextToken();
    boolTerms.push_back(bool_term());
  }

  return new ExprNode(boolTerms);
}

AstNode *Parser::bool_term()
{
  AstNodeList relExprs;

  relExprs.push_back(rel_expr());
  while (currentToken == Token::BOOL_AND)
  {
    currentToken = lexer.nextToken();
    relExprs.push_back(rel_expr());
  }

  return new BoolTermNode(relExprs);
}

AstNode *Parser::rel_expr()
{
  std::string expr = "";
  AstNodeList arithExprs;

  arithExprs.push_back(arith_expr());
  if (currentToken == Token::GT || currentToken == Token::LT || currentToken == Token::GET || currentToken == Token::LET || currentToken == Token::NE || currentToken == Token::EQ)
  {
    expr = currentTokenText();
    currentToken = lexer.nextToken();
    arithExprs.push_back(arith_expr());
  }

  return new RelExprNode(arithExprs, expr);
}

AstNode *Parser::arith_expr()
{
  std::string expr = "";
  AstNodeList arithTerms;

  arithTerms.push_back(arith_term());
  while (currentToken == Token::OP_ADD || currentToken == Token::OP_SUB)
  {
    expr = currentTokenText();
    currentToken = lexer.nextToken();
    arithTerms.push_back(arith_term());
  }

  return new ArithExprNode(arithTerms, expr);
}

AstNode *Parser::arith_term()
{
  std::string expr = "";
  AstNodeList arithFactors;

  arithFactors.push_back(arith_factor());
  while (currentToken == Token::OP_MUL || currentToken == Token::OP_DIV || currentToken == Token::OP_MOD)
  {
    expr = currentTokenText();
    currentToken = lexer.nextToken();
    arithFactors.push_back(arith_factor());
  }

  return new ArithTermNode(arithFactors, expr);
}

AstNode *Parser::arith_factor()
{
  AstNode *factor = defaultNode;

  if (currentToken == Token::OPEN_PAR)
  {
    AstNode *e;

    currentToken = lexer.nextToken();

    e = expr();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    factor = new ParExprNode(e);
  }
  else if (currentToken == Token::IDENT)
  {
    factor = variable();
  }
  else if (currentToken == Token::KW_READ)
  {
    currentToken = lexer.nextToken();

    ErrorEval(Token::OPEN_PAR, "Expected '('");
    currentToken = lexer.nextToken();

    ErrorEval(Token::CLOSE_PAR, "Expected ')'");
    currentToken = lexer.nextToken();

    factor = new ReadNode();
  }
  else if (currentToken == Token::NUMBER)
  {
    std::string numero = currentTokenText();
    currentToken = lexer.nextToken();

    factor = new ConstIntNode(numero);
  }
  else if (currentToken == Token::CLOSE_PAR)
  {
    currentToken = lexer.nextToken();

    factor = new ParEmptyNode();
  }
  else
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": Expected IDENT, INT_LITERAL or '('");
  }

  return new ArithFactorNode(factor);
}

AstNode *Parser::expr_list()
{
  AstNodeList exprList;

  exprList.push_back(expr());
  while (currentToken == Token::COMMA)
  {
    currentToken = lexer.nextToken();
    exprList.push_back(expr());
  }

  return new ExprListNode(exprList);
}

// Detecta si el token esperado es igual al token actual, si no lo es, lanza una excepción
void Parser::ErrorEval(Token expectedToken, const std::string &message)
{
  if (currentToken != expectedToken)
  {
    throw std::runtime_error("Error at line " + std::to_string(lexer.errLine()) + ": " + message);
  }
}
