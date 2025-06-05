#include "ast.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Check if a file argument was provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <brainfuck_file>" << std::endl;
        return 1;
    }

    // Open and read the file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // Read the entire file content
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    std::cout << "Source code:" << std::endl << source << std::endl;
    
    AST ast;
    try {
        ast = parse(source);
        
        // Print the AST for debugging purposes
        std::cout << "AST created with " << ast.exprs.size() << " expressions." << std::endl;

        // create a program with the AST
        Program program(std::move(ast));
        // run the program
        program.run();
        
        std::cout << "Program executed successfully." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}