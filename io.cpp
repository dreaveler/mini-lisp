#include"io.h"
#include"tokenizer.h"
#include"token.h"
#include"parser.h"
#include<fstream>

std::string Input::Lisp(std::shared_ptr<EvalEnv> env, std::string line) {
    auto tokens = Tokenizer::tokenize(line);
    Parser parser(std::move(tokens));  // TokenPtr 不支持复制
    auto value = parser.parse();
    return env->eval(std::move(value))->toString();
}

void Input::processInput(std::shared_ptr<EvalEnv> env) {
    std::string line;
    while (true) {
        processOne();
        if (!std::getline(*in, line)) break;
        try {
            auto result = Lisp(env, line);
            processTwo(result);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}
//简单工厂产出  设为静态
Input* Input::parseArgs(int argc, char** argv) {
    if (argc==1){
        return new ReplInput(&std::cin);
    }

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
        throw SyntaxError("Usage: " + std::string(argv[0]) + " [-i filename]");
    }
    auto file = new std::ifstream(argv[2]);
    if (!file->is_open()) {
        delete file;
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return new FileInput(file);
}

void ReplInput::processOne() {
    std::cout << ">>> ";
    std::cout.flush();
}
void ReplInput::processTwo(std::string result) {
    std::cout << result << "\n";
}
void FileInput::processOne() {}
void FileInput::processTwo(std::string result) {}

Input::~Input() {}
ReplInput::~ReplInput() {}
FileInput::~FileInput() {
    delete in;
}
