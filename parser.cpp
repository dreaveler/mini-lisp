#include "parser.h"
#include "error.h"
#include <iostream>

Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {}
void Parser::checkExtraToken() {
    if (tokens.size()!=1||tokens.at(0)->getType()!=TokenType::RIGHT_PAREN){
        throw SyntaxError("The input is not correct.");
    }
}
void Parser::checkNoneToken() {
    if (tokens.size() != 0) {
        throw SyntaxError("The input is not correct.");
    }
}
ValuePtr Parser::parse() {
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    } 
    else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    } 
    else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    } 
    else if (token->getType() == TokenType::IDENTIFIER) {
        auto name = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(name);
    } 
    else if (token->getType() == TokenType::LEFT_PAREN) {
        return this->parseTails();
    }
    else if (token->getType() == TokenType::QUOTE) {
        return vec2pair({std::make_shared<SymbolValue>("quote"), this->parse()});
    } 
    else if (token->getType() == TokenType::QUASIQUOTE) {
        return vec2pair(
            {std::make_shared<SymbolValue>("quasiquote"), this->parse()});
    } 
    else if (token->getType() == TokenType::UNQUOTE) {
        return vec2pair(
            {std::make_shared<SymbolValue>("unquote"), this->parse()});
    }
    else if (token->getType() == TokenType::RIGHT_PAREN) {
        throw SyntaxError("There is extra right paren.");
    }
    throw SyntaxError("Undefined token.");
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
