#pragma once
#ifndef BUILTIN_H
#define BUILTIN_H

#include<unordered_map>
#include"error.h"
#include "value.h"
#include "eval_env.h"

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
ValuePtr isT(const std::vector<ValuePtr>& params,EvalEnv& env) {
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
    std::vector<ValuePtr> getProcAndList(const std::vector<ValuePtr>& params);
    //核心库
    ValuePtr apply(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr eval(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr exit(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env);
    //对子与列表操作库
    ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env);
    //算数运算库
    ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr minus(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr plus(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr abs(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr remainder(const std::vector<ValuePtr>& params, EvalEnv& env);
    //比较库
    ValuePtr eq(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr no(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr equ(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr less(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr larger(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr lesse(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr largere(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env);
    ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env);
    }

#endif  
