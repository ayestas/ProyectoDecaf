# ProyectoDecaf
Este proyecto tiene como objetivo recrear un compilador del lenguaje Decaf desde cero utilizando una combinación de herramientas y tecnologías modernas. Los componentes clave de este proyecto incluyen:

- CMake: Para gestionar el proceso de construcción y las dependencias.
- re2c: Para generar analizadores léxicos (scanners).
- MIPSx32: Para generar código dirigido a la arquitectura MIPS de 32 bits.

El objetivo es desarrollar un compilador funcional que pueda analizar código Decaf, verificar su sintaxis y generar código ejecutable para la arquitectura MIPSx32. Este proyecto sirve tanto como un ejercicio de aprendizaje como una exploración técnica en la construcción de compiladores y diseño de lenguajes.

# Comandos
Crear /build
- cmake -B build -S .

Build proyecto:
- cmake --build build/

Correr proyecto:
- ./build/Proyecto ./tests/test.cpp

# Descripción del Proyecto
- Seguir las reglas de Decaf en caso de dudas.
- No variables globales. Solo dentro de funciones y al principio y luego las sentencias.
- Tipo de funcion de arreglo no, como parametro si.
- Comentarios de una línea y de bloque.

## Tipos de Datos
- Enteros
- Arreglos de enteros unidimensional

## Operadores Lógicos
- (<) menor que
- (>) mayor que
- (>=) mayor igual que
- (<=) menor igual que
- (!=) diferente
- (==) igual

## Operadores Aritméticos
- (+) suma
- (-) resta
- (*) multiplicación
- (/) división
- (%) módulo
