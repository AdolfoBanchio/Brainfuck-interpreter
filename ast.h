#ifndef AST_H
#define AST_H

#include <vector>
#include <memory>

// Constants
const int MAX_MEMORY = 30000;

// Forward declarations
class Expr;
class AST;

// State struct to hold the program's state
struct State {
    int memory[MAX_MEMORY];
    std::vector<int> inputs;  // Store inputs given to the program
    std::vector<int> outputs; // Store outputs produced by the program
    size_t pointer;

    State() : pointer(0) {
        // Initialize all memory cells to 0
        for (int i = 0; i < MAX_MEMORY; i++) {
            memory[i] = 0;
        }
    }
};

// Class declarations
class Expr {
    public:
        virtual ~Expr() = default;
        virtual void execute(State& state) = 0;
};

class MoveRight : public Expr {
    public:
        void execute(State& state) override;
};

class MoveLeft : public Expr {
    public:
        void execute(State& state) override;
};

class Increment : public Expr {
    public:
        void execute(State& state) override;
};

class Decrement : public Expr {
    public:
        void execute(State& state) override;
};

class Output : public Expr {
    public:
        void execute(State& state) override;
};

class Input : public Expr {
    public:
        void execute(State& state) override;
};

class Loop : public Expr {
    private:
        std::vector<std::unique_ptr<Expr>> body;
    public:
        explicit Loop(std::vector<std::unique_ptr<Expr>>&& body)
            : body(std::move(body)) {};
        void execute(State& state) override;
};

class AST {
    public:
        std::vector<std::unique_ptr<Expr>> exprs;
    public:
        void addExpr(std::unique_ptr<Expr> Expr);
        void execute(State& state);
};

class Program {
    public:
        AST ast;
        State state;
        explicit Program(AST&& ast);
        void run();
};

AST parse(const std::string& code);
std::vector<std::unique_ptr<Expr>> parseLoopBody(const std::string& code, size_t& pos);
#endif // AST_H

