#include"io.h"
#include"tokenizer.h"
#include"token.h"
#include"parser.h"
#include<fstream>
#include<stack>
//bug：输入3 2是正确的没有报错  需要报错
//改进思路  tokens记录左右括号是否能配对  缺右括号的话就继续等待输入  缺左括号直接throw错误
void Input::processInput(std::shared_ptr<EvalEnv> env) {
    std::string line;
    while (true) {
        try {
            std::stack<char> parenContainer;  //parenContainer作为局部变量
            processOne();
            std::getline(*in, line);
            auto tokens = Tokenizer::tokenize(line); 
            Parser parser(std::move(tokens));  // TokenPtr 不支持复制
            auto value = parser.parse();
            parser.checkNoneToken(); //检查是否有残余输入
            auto result = env->eval(std::move(value))->toString();
            processTwo(result);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}
//简单工厂产出  设为静态
std::unique_ptr<Input>Input::parseArgs(int argc, char** argv) {
    if (argc==1){
        return std::make_unique<ReplInput>(&std::cin);
    }

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
        throw SyntaxError("Usage: " + std::string(argv[0]) + " [-i filename]");
    }
    auto file = new std::ifstream(argv[2]);
    if (!file->is_open()) {
        delete file;
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return std::make_unique<FileInput>(file);
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
