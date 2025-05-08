#include"eval_env.h"
#include"error.h"
#include<set>
#include"builtin.h"
#include<algorithm>
#include<iterator>

EvalEnv::EvalEnv(){
    Builtin built;
    for (auto& it:built.builtin_map){
        map.insert({it.first,it.second});
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr){
    if (expr->isSelfEvaluating(expr)) {
        return expr;
    }
    else if (expr->isNil(expr)) {
        throw LispError("Evaluating nil is prohibited.");
    } 
    else if (expr->isSymbol(expr)) {
        return eval_symbol(expr);
    }
    else if (expr->isList(expr)) {
        return eval_list(expr);
    } 
    else if (expr->isPair(expr)) {
        return expr;
    }
    else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::eval_list(ValuePtr expr) {
    using namespace std::literals;
    auto v = expr->toVec(expr);
    if (v[0]->asSymbol(v[0]) == "define"s){
        if (auto name = v[1]->asSymbol(v[1])) {
            ValuePtr current_expr = v[2];
            current_expr = checkVal(current_expr);
            map[name.value()] = current_expr;
        }
        return std::make_shared<NilValue>();
    } else {
        ValuePtr proc = this->eval(v[0]);
        auto pair = dynamic_cast<PairValue*>(expr.get());
        auto cdr = pair->get_cdr();
        std::vector<ValuePtr> args = this->evalList(cdr);
        return this->apply(proc,args);
    }
}
//如果是类型检查库就不改变原有格式直接传入？？

ValuePtr EvalEnv::eval_symbol(ValuePtr expr) {
    if (auto name = expr->asSymbol(expr)) {
        if (auto it = map.find(name.value());it!=map.end()) {
            return it->second;
        } else {
            throw LispError(" Variable " + *name + " not defined.");
        }
    } else {
        throw SyntaxError("It is not a symbol.");
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVec(expr), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}
ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>args) {
    if (auto pro = dynamic_cast<BuiltinProcValue*>(proc.get())) {
        auto func = pro->getFunc();
        return func(args);
    }
    else {
        throw LispError("Unimplemented");
    }
}

//我需要这个函数的定位是传入一个任意类型的ValuePtr   传出一个SelfEvaluating的ValuePtr
ValuePtr EvalEnv::checkVal(ValuePtr expr) {
    while (true){
        if (expr->isSelfEvaluating(expr)||
            expr->isBuiltin(expr)||
            expr->isNil(expr)||
            (!expr->isList(expr)) && (expr->isPair(expr))) {
            return expr;
        }
        expr = eval(expr);
    }
}
