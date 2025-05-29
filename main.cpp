#include "ast.h"
#include <iostream>
#include <memory>
#include <stdexcept>

static const std::string testsource = "+++>++>+";

int main() {
    AST ast;
    int lenght = testsource.length();

    std::cout << testsource << std::endl;
    
    ast = parse(testsource);
    
    // Print the AST for debugging purposes
    std::cout << "AST created with " << ast.exprs.size() << " expressions." << std::endl;

    // create a program with the AST
    Program program(std::move(ast));
    // run the program
    try {
        program.run();
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    std::cout << "Program executed successfully." << std::endl;
    // print the first 5 memory cells for debugging
    std::cout << "Memory cells: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << static_cast<int>(program.state.memory[i]) << " ";
    }
    std::cout << std::endl;
    return 0;
}