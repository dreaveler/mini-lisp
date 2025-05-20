#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <deque>
#include <string>

#include "./token.h"
#include <cctype>
#include <set>
#include <stdexcept>
#include "./error.h"

// 颜色定义
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"


class Tokenizer {
private:
    TokenPtr nextToken(int& pos);
    std::deque<TokenPtr> tokenize();

    std::string input;
    Tokenizer(const std::string& input) : input{input} {}

public:
    static std::deque<TokenPtr> tokenize(const std::string& input);
    static std::string tokenToColoredString(const TokenPtr& token);
};

#endif
