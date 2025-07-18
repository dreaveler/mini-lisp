#include <iostream>
#include <string>
#include<fstream>
#include "./tokenizer.h"
#include "./value.h"
#include "parser.h"
#include "eval_env.h"
#include "rjsj_test.hpp"
#include "io.h"
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


int main(int argc, char** argv) {
    //RJSJ_TEST(TestCtx,Lv2,Lv3,Lv4,Lv5,Lv6,Lv7,Lv7Lib,Sicp);
    try {
        auto input = Inputer::parseArgs(argc, argv);
        auto global = EvalEnv::createGlobal();
        input->processInput(global);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
