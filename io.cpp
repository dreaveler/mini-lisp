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
            bool breakWhile{false};
            bool firstLineFlag{true};
            bool parenFlag{false};  // 初始化全部的标志器
            std::deque<TokenPtr> tokens;
            std::stack<char> parenContainer;  // parenContainer作为局部变量
            while (!parenFlag) {
                if (firstLineFlag) {
                    processOneFirstLine();
                    firstLineFlag = false;
                } else
                    processOneOtherLine();  // 当第一次输入时调用firstline的函数
                                            // 之后调用otherline的函数
                if (!std::getline(*in, line)) {
                    breakWhile = true;
                    break;
                }
                auto code = strip_comment(line);
                auto newTokens = Tokenizer::tokenize(code);  // 解析一次
                tokens.insert(tokens.end(),
                              std::make_move_iterator(newTokens.begin()),
                              std::make_move_iterator(newTokens.end()));
                parenFlag = checkParen(tokens, parenContainer);
            }
            if (breakWhile) break;
            if (tokens.size()==0) continue;
            Parser parser(std::move(tokens));  // TokenPtr 不支持复制
            auto value = parser.parse();
            parser.checkNoneToken();  // 检查是否有残余输入
            auto result = env->eval(std::move(value))->toString();
            processTwo(result);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

// 简单工厂产出  设为静态
std::unique_ptr<Inputer> Inputer::parseArgs(int argc, char** argv) {
    if (argc == 1) {
        return std::make_unique<ReplInputer>(&std::cin);
    }

    if (argc != 3 || std::strcmp(argv[1], "-i") != 0) {
        throw SyntaxError("Usage: " + std::string(argv[0]) + " [-i filename]");
    }
    auto file = new std::ifstream(argv[2]);
    if (!file->is_open()) {
        delete file;
        throw SyntaxError("Cannot open file " + std::string(argv[2]));
    }
    return std::make_unique<FileInputer>(file);
}

void ReplInputer::processOneFirstLine() {
    std::cout << ">>> ";
    std::cout.flush();
}
void ReplInputer::processTwo(std::string result) {
    std::cout << result << "\n";
}
void FileInputer::processOneFirstLine() {}
void FileInputer::processTwo(std::string result) {}

//去除单行注释
std::string Inputer::strip_comment(const std::string& line) {
    auto pos = line.find(';');
    if (pos != std::string::npos) {
        return line.substr(0, pos);
    }
    return line;
}
void ReplInputer::processOneOtherLine() {
    std::cout << "...> ";
    std::cout.flush();
}
void FileInputer::processOneOtherLine() {}
//好长的形参变量名。。
bool Inputer::checkParen(const std::deque<TokenPtr>& tokens, std::stack<char> parenContainer) {
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
