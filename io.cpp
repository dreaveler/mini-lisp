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
            processOne();
            if(!std::getline(*in, line)) break;
            auto code = strip_comment(line);
            auto tokens = Tokenizer::tokenize(code); 
            if (tokens.size()==0) continue;   //输入空行直接过
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

void ReplInputer::processOne() {
    std::cout << ">>> ";
    std::cout.flush();
}
void ReplInputer::processTwo(std::string result) {
    std::cout << result << "\n";
}
void FileInputer::processOne() {}
void FileInputer::processTwo(std::string result) {}

Inputer::~Inputer() {}
ReplInputer::~ReplInputer() {}
FileInputer::~FileInputer() {}
//去除单行注释
std::string Inputer::strip_comment(const std::string& line) {
    auto pos = line.find(';');
    if (pos != std::string::npos) {
        return line.substr(0, pos);
    }
    return line;
}
