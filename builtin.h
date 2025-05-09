#pragma once
#ifndef BUILTIN_H
#define BUILTIN_H
#include"value.h"
#include<unordered_map>

class Builtin {
public:
    std::unordered_map<std::string, ValuePtr> builtin_map;
    Builtin();
};

namespace builtin {
    //工具函数
    std::vector<double> checkAndGetTwoNums(const std::vector<ValuePtr>& params);
    double checkAndGetOneNum(const std::vector<ValuePtr>& params);
    //核心库
    ValuePtr print(const std::vector<ValuePtr>& params);
    ValuePtr display(const std::vector<ValuePtr>& params);
    ValuePtr error(const std::vector<ValuePtr>& params);
    ValuePtr exit(const std::vector<ValuePtr>& params);
    ValuePtr newline(const std::vector<ValuePtr>& params);
    //类型检查库
    ValuePtr isAtom(const std::vector<ValuePtr>& params);
    ValuePtr isBoolean(const std::vector<ValuePtr>& params);
    ValuePtr isInteger(const std::vector<ValuePtr>& params);
    ValuePtr isList(const std::vector<ValuePtr>& params);
    ValuePtr isNumber(const std::vector<ValuePtr>& params);
    ValuePtr isNull(const std::vector<ValuePtr>& params);
    ValuePtr isPair(const std::vector<ValuePtr>& params);
    ValuePtr isProcedure(const std::vector<ValuePtr>& params);
    ValuePtr isString(const std::vector<ValuePtr>& params);
    ValuePtr isSymbol(const std::vector<ValuePtr>& params);
    //对子与列表操作库
    ValuePtr cdr(const std::vector<ValuePtr>& params);
    ValuePtr car(const std::vector<ValuePtr>& params);
    ValuePtr cons(const std::vector<ValuePtr>& params);
    ValuePtr length(const std::vector<ValuePtr>& params);
    ValuePtr list(const std::vector<ValuePtr>& params);
    ValuePtr append(const std::vector<ValuePtr>& params);
    //算数运算库
    ValuePtr add(const std::vector<ValuePtr>& params);
    ValuePtr minus(const std::vector<ValuePtr>& params);
    ValuePtr plus(const std::vector<ValuePtr>& params);
    ValuePtr divide(const std::vector<ValuePtr>& params);
    ValuePtr abs(const std::vector<ValuePtr>& params);
    ValuePtr expt(const std::vector<ValuePtr>& params);
    ValuePtr quotient(const std::vector<ValuePtr>& params);
    ValuePtr modulo(const std::vector<ValuePtr>& params);
    ValuePtr remainder(const std::vector<ValuePtr>& params);
    //比较库
    ValuePtr equ(const std::vector<ValuePtr>& params);
    ValuePtr less(const std::vector<ValuePtr>& params);
    ValuePtr larger(const std::vector<ValuePtr>& params);
    ValuePtr lesse(const std::vector<ValuePtr>& params);
    ValuePtr largere(const std::vector<ValuePtr>& params);
    ValuePtr even(const std::vector<ValuePtr>& params);
    ValuePtr odd(const std::vector<ValuePtr>& params);
    ValuePtr zero(const std::vector<ValuePtr>& params);
    }

#endif  
