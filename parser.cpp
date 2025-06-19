#include "parser.h"
#include "error.h"
#include <iostream>

Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {}
//确保分析完之后没有残余  比如输入 3 2   会报错
void Parser::checkNoneToken() {
    if (tokens.size() != 0) {
        throw SyntaxError("There is extra token。");
    }
}
//重构 通过一个值和一个lambda表达式的const映射来完成这个函数  这样至少不会违反开闭原则
//需要添加token类型对应的操作方式在unordered_map中添加即可
ValuePtr Parser::parse() {
    if (tokens.empty()) throw SyntaxError("Unexpected EOF");
    auto token = std::move(tokens.front());
    tokens.pop_front();
    auto it = handlers.find(token->getType());
    if (it != handlers.end()) {
        return it->second(this, token);
    }
    throw SyntaxError("Unsupported token type");
}
//分析尾序列 应该不用重构吧）
ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw SyntaxError("Missing ')'");
    }
    if (tokens.at(0)->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (!tokens.empty()&&tokens.at(0)->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = this->parse();
        if (tokens.empty()||tokens.at(0)->getType() != TokenType::RIGHT_PAREN)
            throw SyntaxError("Should be a right paren.");
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    } else {
        if (tokens.empty()) {
            throw SyntaxError("Missing ')'");
        }
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}
