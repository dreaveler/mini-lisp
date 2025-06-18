#include"eval_env.h"
#include"error.h"
#include<set>
#include"builtin.h"
#include<algorithm>
#include<iterator>
#include"forms.h"
#include"utils.hpp"

EvalEnv::EvalEnv(std::shared_ptr<EvalEnv>parent = nullptr):parent(parent){
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
    else if (expr->isProcedure(expr)) {
        return expr;
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
}

ValuePtr EvalEnv::eval_list(ValuePtr expr) {
    auto v = toVec(expr);
    auto pair = dynamic_cast<PairValue*>(expr.get());
    auto car = pair->get_car();
    auto cdr = pair->get_cdr();
    auto proc = this->eval(v[0]);
    auto name = proc->asSymbol(proc);
    if (name.has_value()&&SPECIAL_FORMS.contains(*name)) {
            return SPECIAL_FORMS.at(*name)(toVec(cdr), *this);
    } else if (proc->isBuiltin(proc)) {
        std::vector<ValuePtr> args = this->evalList(cdr);
        return this->apply(proc,args);
    } else if (name.has_value() && makesureBinding(*name)||Value::isLambda(proc)) {
        auto args = evalList(cdr);
        return this->apply(proc,args);
    } else
        throw LispError("Should be a procedure.");
}
//如果是类型检查库就不改变原有格式直接传入？？

ValuePtr EvalEnv::eval_symbol(ValuePtr expr) {
    auto name = expr->asSymbol(expr);
    if (name.has_value()&&SPECIAL_FORMS.contains(*name)) {
        return expr;
    } else if(name.has_value()) {
        return lookupBinding(name.value());
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(toVec(expr), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}
ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>args) {
    if (auto pro = dynamic_cast<BuiltinProcValue*>(proc.get())) {
        auto func = pro->getFunc();
        return func(args,*this);
    } else if (auto lambda = dynamic_cast<LambdaValue*>(proc.get())) {
        return lambda->apply(args);
    }
}

ValuePtr EvalEnv::lookupBinding(std::string str) {
    std::shared_ptr<EvalEnv> current {this->shared_from_this()};
    while (current->parent != nullptr) {
        if (auto it = current->map.find(str); it != current->map.end()) {
            return it->second;
        }
        current = current->parent->shared_from_this();
    }
    if (auto it = current->map.find(str); it != current->map.end()) {
        return it->second;
    }
    throw LispError(" Variable " + str + " not defined.");
}
bool EvalEnv::makesureBinding(std::string str) {
    std::shared_ptr<EvalEnv> current{this->shared_from_this()};
    while (current->parent != nullptr) {
        if (auto it = current->map.find(str); it != current->map.end()) {
            return true;
        }
        current = current->parent->shared_from_this();
    }
    if (auto it = current->map.find(str); it != current->map.end()) {
        return true;
    }
    return false;
}
void EvalEnv::defineBinding(std::string str,const ValuePtr& v) {
    map[str] = v;
}
std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    return std::make_shared<EvalEnv>(nullptr);
}
std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args) {
    if (params.size()!=args.size()){
        throw LispError("The size of params is not right");
    }
    std::shared_ptr<EvalEnv> child{std::make_shared<EvalEnv>(this->shared_from_this())};
    for (int i=0;i<params.size();i++){
        child->defineBinding(params[i], args[i]);
    }
    return child;
}
