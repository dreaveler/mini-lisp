#pragma once
#ifndef PARSER_H
#define PARSER_H

#include"token.h"
#include"value.h"
#include"utils.hpp"
#include<deque>
#include <iostream>

class Parser {
private:
    std::deque<TokenPtr> tokens;

public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
    void checkNoneToken();
};
static const std::unordered_map<TokenType,std::function<ValuePtr(Parser*, TokenPtr&)>>
    handlers = {
        {TokenType::NUMERIC_LITERAL,[](Parser*, TokenPtr& token) {
             auto& t = dynamic_cast<NumericLiteralToken&>(*token);
             return std::make_shared<NumericValue>(t.getValue());
         }},
        {TokenType::BOOLEAN_LITERAL,[](Parser*, TokenPtr& token) {
             auto& t = dynamic_cast<BooleanLiteralToken&>(*token);
             return std::make_shared<BooleanValue>(t.getValue());
         }},
        {TokenType::LEFT_PAREN,[](Parser* p, TokenPtr&) { 
         return p->parseTails(); }},
        {TokenType::STRING_LITERAL, [](Parser*,TokenPtr& token){
            auto& t = dynamic_cast<StringLiteralToken&>(*token);
            return std::make_shared<StringValue>(t.getValue());
        }},
        {TokenType::IDENTIFIER, [](Parser*, TokenPtr& token) {
            auto& t = dynamic_cast<IdentifierToken&>(*token);
            return std::make_shared<SymbolValue>(t.getName());
        }},
        {TokenType::QUOTE, [](Parser* p, TokenPtr&) {
            return vec2pair({std::make_shared<SymbolValue>("quote"),p->parse()});
        }},
        {TokenType::UNQUOTE,[](Parser* p, TokenPtr&) {
             return vec2pair({std::make_shared<SymbolValue>("unquote"), p->parse()});
         }},
        {TokenType::UNQUOTE,[](Parser* p, TokenPtr&) {
             return vec2pair({std::make_shared<SymbolValue>("unquote"), p->parse()});
         }},
        {TokenType::QUASIQUOTE,[](Parser* p, TokenPtr&) {
             return vec2pair({std::make_shared<SymbolValue>("quasiquote"), p->parse()});
         }},
        {TokenType::RIGHT_PAREN,
         [](Parser*, TokenPtr&) -> ValuePtr { throw LispError("There is a extra right paren.");
        }}
};

#endif
