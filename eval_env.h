#pragma once
#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include"value.h"
#include<unordered_map>
class EvalEnv {
private:
    std::unordered_map<std::string,ValuePtr>map;
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    ValuePtr eval_list(ValuePtr expr);
    ValuePtr eval_symbol(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc,std::vector<ValuePtr>args);
    ValuePtr checkVal(ValuePtr expr);
};

#endif
