cmake_minimum_required(VERSION 3.14)
project(Proyecto)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program (TREECC treecc
              PATHS /usr /usr/local /opt /opt/local $ENV{TREECC_DIR}
              PATH_SUFFIXES treecc/bin bin )

if (TREECC)
    message(STATUS "Using treecc executable ${TREECC}")
else(TREECC)
    message(FATAL_ERROR "treecc was not found. Try setting TREECC_DIR environment variable")
endif(TREECC)

# Run treecc to generate the AST
add_custom_command (
  OUTPUT Ast.cpp Ast.hpp
  COMMAND ${TREECC} -o Ast.cpp -h Ast.hpp ${PROJECT_SOURCE_DIR}/Ast.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/Ast.tc
  )

add_custom_command(
    OUTPUT LexerImpl.cpp
    COMMAND re2c -o LexerImpl.cpp ${PROJECT_SOURCE_DIR}/Lexer.re 
    MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/Lexer.re
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
  Lexer.hpp 
  LexerImpl.cpp 
  Parser.hpp
  Parser.cpp
  Ast.hpp
  Ast.cpp
  main.cpp
)
