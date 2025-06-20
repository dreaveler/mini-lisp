#include"io.h"
#include"tokenizer.h"
#include"token.h"
#include"parser.h"
#include<fstream>
#include<stack>
//
void Inputer::processInput(std::shared_ptr<EvalEnv> env) {
    Parser parser(std::move(tokens));  // TokenPtr 不支持复制
    auto value = parser.parse();
        processOne();
        if (!std::getline(*in, line)) break;
    return env->eval(std::move(value))->toString();
            bool breakWhile {false};
            bool firstLineFlag {true};
            bool parenFlag {false};     //初始化全部的标志器
            std::deque<TokenPtr>tokens;
            std::stack<char> parenContainer;  //parenContainer作为局部变量
            while (!parenFlag) {
                if (firstLineFlag) {
                    processOneFirstLine();
                    firstLineFlag = false;
                }
                else
                    processOneOtherLine();  //当第一次输入时调用firstline的函数  之后调用otherline的函数
                if (!std::getline(*in, line)) {
                    breakWhile = true;

                    break;
                }
                auto newTokens = Tokenizer::tokenize(line);  // 解析一次
                tokens.insert(tokens.end(),
                              std::make_move_iterator(newTokens.begin()),
                              std::make_move_iterator(newTokens.end()));
                parenFlag = checkParen(tokens,parenContainer);
            }
            if (breakWhile) break;
            Parser parser(std::move(tokens));  // TokenPtr 不支持复制
            auto value = parser.parse();
            parser.checkNoneToken(); //检查是否有残余输入
            auto result = env->eval(std::move(value))->toString();

void Input::processInput(std::shared_ptr<EvalEnv> env) {
    std::string line;
    while (true) {
        try {
            auto result = Lisp(env, line);
            processTwo(result);
std::unique_ptr<Inputer>Inputer::parseArgs(int argc, char** argv) {
            std::cerr << "Error: " << e.what() << std::endl;
        return std::make_unique<ReplInputer>(&std::cin);
    }
}
//简单工厂产出  设为静态
Input* Input::parseArgs(int argc, char** argv) {
    if (argc==1){
        return new ReplInput(&std::cin);
    }

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
    return std::make_unique<FileInputer>(file.release());
    }
    auto file = std::make_unique<std::ifstream>(argv[2]);
void ReplInputer::processOne() {
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return new FileInput(file);
}

void ReplInput::processOne() {
void FileInputer::processOne() {}
void FileInputer::processTwo(std::string result) {}
}
void ReplInputer::processTwo(std::string result) {
    std::cout << result << "\n";
}
void FileInput::processOne() {}
void FileInput::processTwo(std::string result) {}

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
void ReplInput::processOneOtherLine() {
    std::cout << "...> ";
    std::cout.flush();
}
void FileInput::processOneOtherLine() {}
//好长的形参变量名。。
bool Input::checkParen(const std::deque<TokenPtr>& tokens, std::stack<char> parenContainer) {
    for (const auto& token : tokens) {
        if (token->getType() == TokenType::LEFT_PAREN) {
            parenContainer.push('('); 
        } else if (token->getType() == TokenType::RIGHT_PAREN) {
            if (!parenContainer.empty() && parenContainer.top() == '(') {
                parenContainer.pop();
            } else {
                throw SyntaxError("Unmatched right parenthesis detected.");
            }
        }
    }
    if (parenContainer.empty()) return true;
    else return false;
}
