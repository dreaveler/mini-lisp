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
    {"lambda",lambdaForm}
    };

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol(args[0])) {
        if (args.size() != 2) throw LispError("Should get two param.");
        ValuePtr current_expr = args[1];
        current_expr = env.checkVal(current_expr);
        env.defineBinding(*name,current_expr);
        return std::make_shared<NilValue>();
    }
    else if (Value::isList(args[0])) {
        auto expr = Value::toVec(args[0]);
        auto first = vec2pair(expr);
        auto funcname = expr[0]->asSymbol(expr[0]);
        auto pair = dynamic_cast<PairValue*>(first.get());
        auto params = pair->get_cdr();
        std::vector<ValuePtr> input;
        std::copy(args.begin()+1,args.end(),std::back_inserter(input));
        input.insert(input.begin(),params);
        input.insert(input.begin() ,std::make_shared<SymbolValue>("lambda"));
        auto arg = vec2pair(input);
        arg = env.checkVal(arg);
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
    auto& params = args[0];
    auto vecOfParams = Value::toVec(params);
    std::vector<std::string>vecStrParams;
    std::ranges::transform(vecOfParams, std::back_inserter(vecStrParams),
                           [](const ValuePtr& v) { return v->asSymbol(v).value();});
    std::vector<ValuePtr> body;            
    std::copy(args.begin() + 1, args.end(),std::back_inserter(body));
    return std::make_shared<LambdaValue>(vecStrParams,body,env.shared_from_this());
}
