#include"eval_env.h"
#include"error.h"
#include<set>

ValuePtr EvalEnv::eval(ValuePtr expr){
    if (expr->isSelfEvaluating(expr)) {
        return expr;
    }
    else if (expr->isNil(expr)) {
        throw LispError("Evaluating nil is prohibited.");
    } 
    else if (expr->isList(expr)) {
        return eval_list(expr);
    }
    else if (expr->isSymbol(expr)) {
        return eval_symbol(expr);
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
            std::set<std::string> visited;
            while (true) {
                auto current_name = current_expr->asSymbol(current_expr);
                if (!current_name) {
                    break;
                }
                if (visited.count(current_name.value())) {
                    throw LispError("Circular reference detected for symbol: " +
                                    *current_name);
                }
                visited.insert(current_name.value());
                auto it = map.find(current_name.value());
                if (it == map.end()) {
                    throw LispError("Variable " + *current_name +
                                    " not defined.");
                }
                current_expr = it->second;
            }
            map[name.value()] = current_expr;
        }
        return std::make_shared<NilValue>();
    }
    else {
        throw LispError("Malformed define.");
    }
}
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
