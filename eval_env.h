#pragma once
#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include"value.h"
#include<unordered_map>
class EvalEnv {
private:
    std::unordered_map<std::string,ValuePtr>map;
public:
    ValuePtr eval(ValuePtr expr);
    ValuePtr eval_list(ValuePtr expr);
    ValuePtr eval_symbol(ValuePtr expr);
};

#endif
