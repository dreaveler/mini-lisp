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
    // 理论上来说应该放在private的  但没有理解为什么它放在private之后会报错
    EvalEnv(std::shared_ptr<EvalEnv> parent); 
    static std::shared_ptr<EvalEnv> createGlobal(); //创建全局环境工厂
    //创建子环境工厂
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params,const std::vector<ValuePtr>& args);
    //求值 只求一次
    ValuePtr eval(ValuePtr expr);
    //对列表求值 直到末尾
    ValuePtr eval_list(ValuePtr expr);
    //对变量/表达式求值
    ValuePtr eval_symbol(ValuePtr expr);
    //对列表的每个值进行一次求值 并以vector的形式返回
    std::vector<ValuePtr> evalList(ValuePtr expr);
    //将内置过程与参数连接来求值
    ValuePtr apply(ValuePtr proc,std::vector<ValuePtr>args);
    //查找变量 返回不可自我求值的值
    ValuePtr lookupBinding(std::string str);
    //确认变量在变量表中
    bool makesureBinding(std::string str);
    //定义变量
    void defineBinding(std::string str,const ValuePtr& v);
};

#endif
