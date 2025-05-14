#pragma once
#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include"value.h"
#include<unordered_map>
class EvalEnv: public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string,ValuePtr>map;
    std::shared_ptr<EvalEnv> parent ;


public:
    EvalEnv(std::shared_ptr<EvalEnv> parent);
    static std::shared_ptr<EvalEnv> createGlobal();
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params,const std::vector<ValuePtr>& args);
    ValuePtr eval(ValuePtr expr);
    ValuePtr eval_list(ValuePtr expr);
    ValuePtr eval_symbol(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc,std::vector<ValuePtr>args);
    ValuePtr checkVal(ValuePtr expr);

    ValuePtr lookupBinding(std::string str);
    bool makesureBinding(std::string str);
    void defineBinding(std::string str,const ValuePtr& v);
};

#endif
