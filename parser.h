#pragma once  
#ifndef PARSER_H  
#define PARSER_H  

#include "token.h"  
#include "value.h"  
#include "utils.hpp"  
#include <deque>  
#include <iostream>  
#include <unordered_map>  
#include <functional>  

class Parser {  
private:  
    std::deque<TokenPtr> tokens;  
public:  
    Parser(std::deque<TokenPtr> tokens);  
    ValuePtr parse();  
    ValuePtr parseTails();  
    void checkNoneToken();  
};  

const std::unordered_map<TokenType, std::function<ValuePtr(Parser*, TokenPtr&)>> handlers = {  
    {TokenType::NUMERIC_LITERAL, [](Parser*, TokenPtr& token) {  
         auto& t = dynamic_cast<NumericLiteralToken&>(*token);  
         return std::make_shared<NumericValue>(t.getValue());  
     }},  
    {TokenType::BOOLEAN_LITERAL, [](Parser*, TokenPtr& token) {  
         auto& t = dynamic_cast<BooleanLiteralToken&>(*token);  
         return std::make_shared<BooleanValue>(t.getValue());  
     }},  
    {TokenType::LEFT_PAREN, [](Parser* p, TokenPtr&) -> ValuePtr {  
         return p->parseTails();  
     }},  
    {TokenType::STRING_LITERAL, [](Parser* p, TokenPtr& token) -> ValuePtr {  
         auto& t = dynamic_cast<StringLiteralToken&>(*token);  
         return std::make_shared<StringValue>(t.getValue());  
     }},  
    {TokenType::IDENTIFIER, [](Parser* p, TokenPtr& token) -> ValuePtr {  
         auto& t = dynamic_cast<IdentifierToken&>(*token);  
         return std::make_shared<SymbolValue>(t.getName());  
     }},  
    {TokenType::QUOTE, [](Parser* p, TokenPtr&) {  
         return vec2pair({std::make_shared<SymbolValue>("quote"), p->parse()});  
     }},  
    {TokenType::QUASIQUOTE, [](Parser* p, TokenPtr&) {  
         return vec2pair({std::make_shared<SymbolValue>("quasiquote"), p->parse()});  
     }},  
    {TokenType::UNQUOTE, [](Parser* p, TokenPtr&) {  
         return vec2pair({std::make_shared<SymbolValue>("unquote"), p->parse()});  
     }},  
    {TokenType::RIGHT_PAREN, [](Parser*, TokenPtr&) ->ValuePtr{  
         throw SyntaxError("There is extra right paren.");  
     }}  
};  

#endif
