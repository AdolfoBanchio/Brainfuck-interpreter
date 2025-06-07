# Brainfuck-interpreter

## Introducción

Este proyecto implementa un **intérprete del lenguaje Brainfuck**, un lenguaje esotérico minimalista compuesto por solo ocho comandos. A pesar de su simplicidad, Brainfuck es turing completo, lo que lo convierte en un excelente caso de estudio para explorar la implementación de un lenguaje formal desde su gramática hasta su semántica operacional.

Un programa de Brainfuck tiene a disposición un puntero movible para manipular un arreglo o "cinta de memoria" de al menos 30.000 celdas. Cada celda contiene un valor entero (de usualmente un byte), tal que su valor inicial es 0 y el puntero comienza en la celda más a la izquierda.

### Comandos del lenguaje

| Comando | Descripción                    |
|--------:|--------------------------------|
| `>`     | Mueve el puntero de memoria a la derecha |
| `<`     | Mueve el puntero de memoria a la izquierda |
| `+`     | Incrementa el valor en la celda actual     |
| `-`     | Decrementa el valor en la celda actual     |
| `.`     | Imprime el carácter en la celda actual     |
| `,`     | Lee un carácter de entrada y lo guarda en la celda actual |
| `[`     | Inicio de bucle: entra si el valor actual es distinto de 0 |
| `]`     | Fin de bucle: vuelve al inicio si el valor actual es distinto de 0 |

---

## Gramática abstracta

Se define la siguiente gramática abstracta para Brainfuck:

    Prog ::= Instr*
    Instr ::= MoveRight | MoveLeft | Increment | Decrement | Output | Input | Loop(Prog)


La sintaxis abstracta está representada mediante una jerarquía de clases C++ derivadas de una clase base `Expr`, que permite construir el árbol de sintaxis abstracta (AST) del programa.

---

## Árbol de Sintaxis Abstracta (AST)

El AST del lenguaje se representa como una **secuencia de expresiones**, donde cada instrucción del lenguaje corresponde a un nodo concreto. La clase `Loop` es el único nodo con un cuerpo anidado (es decir, sub-árbol).

### Estructura de clases principales

- `Expr`: clase abstracta base con el método virtual `eval`.
- `MoveRight`, `MoveLeft`, `Increment`, `Decrement`, `Output`, `Input`: clases concretas que implementan instrucciones simples.
- `Loop`: clase que representa un bucle `[...]`, contiene un sub-AST como cuerpo.
- `AST`: contenedor que representa un programa como una lista de expresiones.
- `Program`: representa la ejecución de un AST sobre un estado inicial de memoria.

---

## Estado y semántica operacional

La semántica está implementada de manera estructurada donde cada clase concreta de `Expr` implementa su comportamiento mediante el método `eval(ProgramState&)`.

### Estado

El estado del programa (`ProgramState`) consiste en:
- Un vector de celdas de memoria (`std::vector<uint8_t>`).
- Un puntero de posición actual.
- Pila que contiene todos los inputs dados al programa
- Pila que contiene todos los outputs dados por el programa

### Semántica operacional

La semántica de cada nodo depende exclusivamente de su **estructura sintáctica** y del **estado del programa**. Por ejemplo:

- `Increment::eval(state)` actualiza la celda actual con `+1`.
- `Loop::eval(state)` evalúa recursivamente su cuerpo mientras el valor actual sea distinto de 0.

---

## Implementación del parser e intérprete

El parser realiza un recorrido lineal del código fuente y genera el AST en una sola pasada. Para cada carácter válido, se construye la expresión correspondiente. En el caso de los bucles (`[` y `]`), se invoca recursivamente `parseLoopBody()` para construir el subárbol.

Si bien la gramatica asbtracta del lenguaje no fue implementada explicitamente, la funcion que se encarga de parsear el codigo fuente, hace uso de esta gramatica abstracta de manera implicita.

```cpp
AST parse(const std::string& code) {
    //... code
        switch (c) {
            case '>': ast.addExpr(std::make_unique<MoveRight>()); break;
            case '<': ast.addExpr(std::make_unique<MoveLeft>()); break;
            case '+': ast.addExpr(std::make_unique<Increment>()); break;
            case '-': ast.addExpr(std::make_unique<Decrement>()); break;
            case '.': ast.addExpr(std::make_unique<Output>()); break;
            case ',': ast.addExpr(std::make_unique<Input>()); break;
            case '[': {
                auto body = parseLoopBody(code, pos);
                ast.addExpr(std::make_unique<Loop>(std::move(body)));
                break;
            }
            case ']':
                throw std::runtime_error("Syntax error: Unmatched closing bracket");
        }
    //... code
```

El objeto `Program` encapsula tanto el AST como el estado inicial y ejecuta el programa mediante una iteración del árbol, respetando la semántica estructurada.

---

## Compilación y uso

### Requisitos previos

- Compilador C++ compatible con C++17 (g++ 7.0 o superior)
- Make (para usar el sistema de build)

### Compilación

Para compilar el proyecto, simplemente ejecute:

```bash
make
```

Esto generará el ejecutable `brainfuck` en el directorio raíz del proyecto.

Para limpiar los archivos generados durante la compilación:

```bash
make clean
```

Dentro de la carpeta tests se encuentran diferentes programas para verificar ciertos comportamientos. El archivo run_tests.sh ejecuta todos los tests y muestra el resultado de cada uno. Dentro de `tests/test_expl.txt` se encuentran los tests explicados.

### Uso

El intérprete puede ejecutar programas de Brainfuck de dos maneras:

1. **Desde un archivo**:
```bash
./brainfuck archivo.b
```

2. **Desde la entrada estándar**:
```bash
./brainfuck
```

### Ejemplos

1. Programa "Hello World" en Brainfuck:
```brainfuck
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```

2. Programa que lee un carácter y lo imprime:
```brainfuck
,.
```

### Notas

- El intérprete utiliza un arreglo de 30,000 celdas inicializadas en 0
- Los valores de las celdas son de 8 bits (0-255)
- Los caracteres de entrada/salida se manejan como ASCII
