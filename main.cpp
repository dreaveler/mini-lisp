#include <iostream>
#include <string>
#include<fstream>
#include "./tokenizer.h"
#include "./value.h"
#include "parser.h"
#include "eval_env.h"
#include "rjsj_test.hpp"
struct TestCtx {
    std::shared_ptr<EvalEnv> global{EvalEnv::createGlobal()};
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = global->eval(std::move(value));
        return result->toString();
    }
};

static std::string Lisp(std::shared_ptr<EvalEnv> env,std::string line) {
    auto tokens = Tokenizer::tokenize(line);
    Parser parser(std::move(tokens));  // TokenPtr 不支持复制
    auto value = parser.parse();
    return env->eval(std::move(value))->toString();
}

static void processInput(std::shared_ptr<EvalEnv> env, std::istream& input,
                         bool isRepl) {
    std::string line;
    while (true) {
        if (isRepl) {
            std::cout << ">>> "; 
            std::cout.flush();
        }
        if (!std::getline(input, line)) break;

        try {
            auto result = Lisp(env, line); 
            if (isRepl) {
                std::cout << result << "\n";  
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}


static std::istream* parseArgs(int argc, char** argv, bool& isRepl) {
    isRepl = (argc == 1);
    if (isRepl) return &std::cin;

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
        throw SyntaxError("Usage: " + std::string(argv[0]) + " [-i filename]");
    }
    auto file = new std::ifstream(argv[2]);
    if (!file->is_open()) {
        delete file;
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return file;
}

int main(int argc, char** argv) {
    try {
        bool isRepl;
        auto inputStream = parseArgs(argc, argv, isRepl);
        auto global = EvalEnv::createGlobal();
        processInput(global, *inputStream, isRepl);
        if (!isRepl) delete inputStream;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
