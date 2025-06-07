/* 
AST (Abstract Syntax Tree) implementation for a simple programming language.

Brainfuck-like syntax with basic constructs.
*/
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "ast.h"

// ⟨ > , σ ⟩ → σ' donde σ' es σ con el puntero incrementado en 1 (si no excede el límite).
void MoveRight::execute(State& state) {
    // check out of bounds, if so terminate excution
    if (state.pointer < MAX_MEMORY - 1) {
        state.pointer++;
    } else {
        throw std::out_of_range("Pointer moved out of bounds");
    }
}

// ⟨ < , σ ⟩ → σ' donde σ' es σ con el puntero decrementado en 1 (si no es menor a 0).
void MoveLeft::execute(State& state) {
    // check out of bounds, if so terminate excution
    if (state.pointer > 0) {
        state.pointer--;
    } else {
        throw std::out_of_range("Pointer moved out of bounds");
    }
}

// ⟨ + , σ ⟩ → σ' donde σ' es σ con el valor de la celda apuntada incrementado en 1 (si no excede el límite).
void Increment::execute(State& state) {
    // Increment the current memory cell, wrap around if necessary
    state.memory[state.pointer] = (state.memory[state.pointer] + 1) % 256;
}

// ⟨ - , σ ⟩ → σ' donde σ' es σ con el valor de la celda apuntada decrementado en 1 (si no excede el límite).
void Decrement::execute(State& state) {
    // Decrement the current memory cell, wrap around if necessary
    state.memory[state.pointer] = (state.memory[state.pointer] - 1 + 256) % 256;
}

// ⟨ . , σ ⟩ → σ con efecto secundario de imprimir el valor de la celda actual como carácter.
void Output::execute(State& state) {
    // If the value is 10, output a newline, otherwise output the character
    if (state.memory[state.pointer] == 10) {
        std::cout << '\n';
        state.outputs.push_back(10);
    } else {
        std::cout << static_cast<char>(state.memory[state.pointer]);
        state.outputs.push_back(state.memory[state.pointer]);
    }
    // Flush the output to ensure immediate display
    std::cout.flush();
}

// ⟨ , σ ⟩ → σ' donde σ' es σ con el valor de la celda apuntada reemplazado por el valor de la entrada.
void Input::execute(State& state) {
    // Read a single character from input
    int c = std::cin.get();
    
    // Handle EOF
    if (c != EOF) {
        // If it's a newline, store 10, otherwise store the character
        state.memory[state.pointer] = (c == '\n') ? 10 : c;
        state.inputs.push_back(c);
    }
}

// ⟨ [ body ] , σ ⟩ → σ' si valor actual ≠ 0, ejecutando cuerpo recursivamente hasta que sea 0.
void Loop::execute(State& state) {
    while (state.memory[state.pointer] != 0) {
        for (auto& cmd : body) {
            cmd->execute(state);
        }
    }
}

// AST class implementation
void AST::addExpr(std::unique_ptr<Expr> expr) {
    exprs.push_back(std::move(expr));
}

void AST::execute(State& state) {
    // Execute all commands in the AST
    for (const auto& expr : exprs) {
        expr->execute(state);
    }
}

std::vector<std::unique_ptr<Expr>> parseLoopBody(const std::string& code, size_t& pos) {
    std::vector<std::unique_ptr<Expr>> result;

    while (pos < code.length()) {
        char c = code[pos++]; // increment here, only once

        switch (c) {
            case '>': result.push_back(std::make_unique<MoveRight>()); break;
            case '<': result.push_back(std::make_unique<MoveLeft>()); break;
            case '+': result.push_back(std::make_unique<Increment>()); break;
            case '-': result.push_back(std::make_unique<Decrement>()); break;
            case '.': result.push_back(std::make_unique<Output>()); break;
            case ',': result.push_back(std::make_unique<Input>()); break;
            case '[': {
                auto body = parseLoopBody(code, pos);
                result.push_back(std::make_unique<Loop>(std::move(body)));
                break;
            }
            case ']':
                return result;  // we return to the caller when loop ends
        }
    }

    throw std::runtime_error("Syntax error: Unmatched opening bracket");
}

AST parse(const std::string& code) {
    AST ast;
    size_t pos = 0;

    while (pos < code.length()) {
        char c = code[pos++]; // increment once at the top

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
    }

    return ast;
}

// Program class implementation
Program::Program(AST&& ast) : ast(std::move(ast)) {
    // Initialize the program state
    state = State();
}

void Program::run() {
    // Execute the AST with the current state
    ast.execute(state);
}

