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
    void checkExtraToken();
    void checkNoneToken();

};

#endif
