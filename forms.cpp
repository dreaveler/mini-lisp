#include"forms.h"
#include<algorithm>
#include<iterator>
#include<iostream>
#include"utils.hpp"
#include<cmath>

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote",quoteForm}, 
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"lambda",lambdaForm},
    {"cond", condForm},     
    {"begin",beginForm}, 
    {"let",letForm},       
    {"quasiquote",quasiquoteForm},
    };
    //define特殊形式 
ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0)
        throw LispError("Should get at least one param.");
    if (auto name = args[0]->asSymbol(args[0])) {
        if (args.size() != 2) throw LispError("Should get two param.");
        ValuePtr current_expr = args[1];
        auto value = env.eval(current_expr);
        env.defineBinding(*name,value);
        return std::make_shared<NilValue>();
    }
    else if (Value::isList(args[0])) {
        auto expr = toVec(args[0]);
        auto first = vec2pair(expr);
        auto funcname = expr[0]->asSymbol(expr[0]);
        auto pair = dynamic_cast<PairValue*>(first.get());
        auto params = pair->get_cdr();
        std::vector<ValuePtr> input;
        std::copy(args.begin()+1,args.end(),std::back_inserter(input));
        input.insert(input.begin(),params);
        input.insert(input.begin() ,std::make_shared<SymbolValue>("lambda"));
        auto arg = vec2pair(input);
        arg = env.eval(arg);
        env.defineBinding(*funcname,arg);
        return std::make_shared<NilValue>();
    }
    else {
        throw LispError("Unimplemented");
    }
}
ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size()!=1){
        throw LispError("Should get one param");
    }
    return args[0];
}
ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) throw LispError("Should get 3 params.");
    if (env.eval(args[0])->asBoolean(env.eval(args[0]))) {
        return env.eval(args[1]);
    }
    else {
        return env.eval(args[2]);
    }
}
ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    ValuePtr last;
    for (auto& param : args) {
        auto t = env.eval(param);
        if (t->asBoolean(t) == false) {
            return std::make_shared<BooleanValue>(false);
        }
        last = t;
    }
    return last;
}
ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto& param : args) {
        auto t = env.eval(param);
        if (t->asBoolean(t) != false) {
            return t;
        }
    }
    return std::make_shared<BooleanValue>(false);
}
//args[0]是形参列表 后面是body
ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2){
        throw LispError("Should get more than one param.");
    }
    if (!Value::isList(args[0])) throw LispError("Define a lambda need the params is a list.");
    auto& params = args[0];
    auto vecOfParams = toVec(params);
    std::vector<std::string>vecStrParams;
    std::ranges::transform(vecOfParams, std::back_inserter(vecStrParams),
                           [](const ValuePtr& v) { return v->asSymbol(v).value();});
    std::vector<ValuePtr> body;            
    std::copy(args.begin() + 1, args.end(),std::back_inserter(body));
    return std::make_shared<LambdaValue>(vecStrParams,body,env.shared_from_this());
}
//args中的每一个是一个子句  子句的[0]是一个条件  后面是表达式
ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    using namespace std::literals;
    for (auto i = 0;i<args.size();i++){
        auto sentence = toVec(args[i]);
        if (sentence.size() == 0) throw LispError("Not defined.");
        if (auto name = sentence[0]->asSymbol(sentence[0]);
            name.has_value() && name == "else"s) {
            if (i != args.size() - 1) throw LispError("The location of else is not right.");
            std::vector<ValuePtr>result;
            std::ranges::transform(sentence.begin() + 1, sentence.end(),
                                   std::back_inserter(result),
                                   [&env](ValuePtr& v) {return env.eval(v);});
            return result[result.size()-1];
        }
        if (auto res = env.eval(sentence[0]); res->asBoolean(res)) {
            std::vector<ValuePtr> result;
            std::ranges::transform(sentence.begin() + 1, sentence.end(),
                                   std::back_inserter(result),
                                   [&env](ValuePtr& v) { return env.eval(v); });
            if (result.size() != 0) {
                return result[result.size()-1];
            }else return res;
        }
    }
}
ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto list = args;
    std::vector<ValuePtr> result;
    std::ranges::transform(list,std::back_inserter(result),
                           [&env](ValuePtr& v) { return env.eval(v); });
    return result[result.size()-1];
}
ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto params = toVec(args[0]);
    std::vector<ValuePtr>body;
    std::copy(args.begin() + 1, args.end(), std::back_inserter(body));
    std::vector<std::string>nameParams;
    std::vector<ValuePtr>trueParams;
    for (auto& param:params){
        auto pair = toVec(param);
        nameParams.push_back(pair[0]->asSymbol(pair[0]).value());
        trueParams.push_back(env.eval(pair[1]));
    }
    auto lambenv = std::make_shared<LambdaValue>(nameParams, body, env.shared_from_this());
    return lambenv->apply(trueParams);
}
ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    using namespace std::literals;
    if (args.size() != 1) throw LispError("Should get one expr.");
    auto& expr = args[0];
    if (!Value::isPair(expr)) return expr;
    auto pair = dynamic_cast<PairValue*>(expr.get());
    auto car = pair->get_car(), cdr = pair->get_cdr();
    auto name = car->asSymbol(car);
    if (name.has_value() && name == "quasiquote"s) throw LispError("The quasiquotein quosiquote is not defined.");
    if (name.has_value() && name == "unquote"s) return env.eval(dynamic_cast<PairValue*>(cdr.get())->get_car());
    ValuePtr qq_car = quasiquoteForm({car}, env);
    ValuePtr qq_cdr = quasiquoteForm({cdr}, env);
    return std::make_shared<PairValue>(qq_car, qq_cdr);
 }
