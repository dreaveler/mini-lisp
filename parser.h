#pragma once
#ifndef PARSER_H
#define PARSER_H

#include"token.h"
#include"value.h"

#include<deque>
#include<vector>
class Parser {
private:
    std::deque<TokenPtr> tokens;

public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};

namespace parser {
    ValuePtr vec2pair(std::vector<ValuePtr> v);
}

#endif
