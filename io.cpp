#include"io.h"
#include"tokenizer.h"
#include"token.h"
#include"parser.h"
#include<fstream>
#include<stack>
//
void Inputer::processInput(std::shared_ptr<EvalEnv> env) {
    std::string line;
    while (true) {
        try {
            std::stack<char> parenContainer;  //parenContainer作为局部变量
            processOne();
            if(!std::getline(*in, line)) break;
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
std::unique_ptr<Inputer>Inputer::parseArgs(int argc, char** argv) {
    if (argc==1){
        return std::make_unique<ReplInputer>(&std::cin);
    }

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
        throw SyntaxError("Usage: " + std::string(argv[0]) + " [-i filename]");
    }
    auto file = std::make_unique<std::ifstream>(argv[2]);
    if (!file->is_open()) {
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return std::make_unique<FileInputer>(file.release());
}
//多态的两种操作模式
void ReplInputer::processOne() {
    std::cout << ">>> ";
    std::cout.flush();
}
void ReplInputer::processTwo(std::string result) {
    std::cout << result << "\n";
}
void FileInputer::processOne() {}
void FileInputer::processTwo(std::string result) {}

