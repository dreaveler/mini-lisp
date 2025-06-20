#include "parser.h"
#include "error.h"
#include <iostream>

Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {}
void Parser::checkNoneToken() {
    if (tokens.size() != 0) {
        throw SyntaxError("The input is not correct.");
    }
}
ValuePtr Parser::parse() {
    auto token = std::move(tokens.front());
    tokens.pop_front();
    auto it = handlers.find(token->getType());
    if (it != handlers.end()) {
        return it->second(this, token);
    }
    throw SyntaxError("Unsupported token type");
}

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
