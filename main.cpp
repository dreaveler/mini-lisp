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

int main(int argc,char** argv) {
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6,Lv7,Lv7Lib,Sicp);
    std::shared_ptr<EvalEnv>global{ EvalEnv::createGlobal()};
    bool REPL {argc==1};
    if (REPL) {
        while (true) {
            try {
                std::cout << ">>> ";
                std::string line;
                std::getline(std::cin, line);
                if (std::cin.eof()) {
                    std::exit(0);
                }
                auto result = Lisp(global, line);
                std::cout << result << "\n";
                }catch (std::runtime_error& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        try {
        
        if (argc != 3)
            throw SyntaxError("You should input *.exe -i (the address).");
        if (std::strcmp(argv[1], "-i")!=0)
            throw SyntaxError(std::string(argv[1]) + " is invalid.");
        std::ifstream file{argv[2]};
        if (!file) throw SyntaxError("The file is not opened.");
        std::string line;
        while (std::getline(file, line)) {
            auto result = Lisp(global, line);
        }
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
    }
}
}
