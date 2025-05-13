#pragma once
#ifndef BUILTIN_H
#define BUILTIN_H

#include<unordered_map>
#include"error.h"
#include "value.h"

class Builtin {
public:
    std::unordered_map<std::string, ValuePtr> builtin_map;
    Builtin();
};


#define CHECKONEPARAM(params)                                     \
    do {                                                          \
        if ((params).size() != 1) {                               \
            throw LispError("The size of params is not correct"); \
        }                                                         \
    } while (0)

#define CHECKDIVIDEND(value)\
    do {   \
        if (value == 0) throw LispError("Cannot divide a zero value"); \
    } while (0)   \

using isFunc = bool(const ValuePtr&);
// 类型检查库——通过非类型模板参数函数实现
template <isFunc* func>
ValuePtr isT(const std::vector<ValuePtr>& params) {
    CHECKONEPARAM(params);
    auto& param = params[0];
    return std::make_shared<BooleanValue>(func(param));
}

namespace builtin {
    //工具函数
    std::vector<double> checkAndGetTwoNums(const std::vector<ValuePtr>& params);
    double checkAndGetOneNum(const std::vector<ValuePtr>& params);
    PairValue* getPair(const std::vector<ValuePtr>& params);
    int getOneInteger(const std::vector<ValuePtr>& params);
    //核心库
    ValuePtr print(const std::vector<ValuePtr>& params);
    ValuePtr display(const std::vector<ValuePtr>& params);
    ValuePtr error(const std::vector<ValuePtr>& params);
    ValuePtr exit(const std::vector<ValuePtr>& params);
    ValuePtr newline(const std::vector<ValuePtr>& params);
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
