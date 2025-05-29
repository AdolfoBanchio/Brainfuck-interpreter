/* 
AST (Abstract Syntax Tree) implementation for a simple programming language.

Brainfuck-like syntax with basic constructs.
*/
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "ast.h"


void MoveRight::execute(State& state) {
    // check out of bounds, if so terminate excution
    if (state.pointer < MAX_MEMORY - 1) {
        state.pointer++;
    } else {
        throw std::out_of_range("Pointer moved out of bounds");
    }
}

void MoveLeft::execute(State& state) {
    // check out of bounds, if so terminate excution
    if (state.pointer > 0) {
        state.pointer--;
    } else {
        throw std::out_of_range("Pointer moved out of bounds");
    }
}

void Increment::execute(State& state) {
    // Increment the current memory cell, wrap around if necessary
    state.memory[state.pointer] = (state.memory[state.pointer] + 1) % 256;
}

void Decrement::execute(State& state) {
    // Decrement the current memory cell, wrap around if necessary
    state.memory[state.pointer] = (state.memory[state.pointer] - 1 + 256) % 256;
}

void Output::execute(State& state) {
    // Output the current memory cell (for simplicity, just print it)
    std::cout << static_cast<int>(state.memory[state.pointer]) << std::endl;
}

void Input::execute(State& state) {
    // Input a value into the current memory cell (for simplicity, just set it to 0)
    std::cout << "Input value for memory cell " << state.pointer << ": ";
    std::cin >> state.memory[state.pointer];
}

void Loop::execute(State& state) {
    while (state.memory[state.pointer] != 0) {
        for (auto& cmd : body) {
            cmd->execute(state);
        }
    }
}

void AST::addExpr(std::unique_ptr<Expr> expr) {
    exprs.push_back(std::move(expr));
}

void AST::execute(State& state) {
    // Execute all commands in the AST
    for (const auto& expr : exprs) {
        expr->execute(state);
    }
}

Program::Program(AST&& ast) : program(std::move(ast)) {
    // Initialize the program state
    state = State();
}

void Program::run() {
    // Execute the AST with the current state
    program.execute(state);
}