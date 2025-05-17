#include"builtin.h"
#include"error.h"
#include<iostream>
#include<cmath>
#include"utils.hpp"
#include<algorithm>
#include<iterator>

Builtin::Builtin() {
    // 核心库 done
    builtin_map.insert({"apply", std::make_shared<BuiltinProcValue>(&builtin::apply)});
    builtin_map.insert({"display", std::make_shared<BuiltinProcValue>(&builtin::display)});
    builtin_map.insert({"displayln", std::make_shared<BuiltinProcValue>(&builtin::displayln)});
    builtin_map.insert({"error", std::make_shared<BuiltinProcValue>(&builtin::error)});
    builtin_map.insert({"eval", std::make_shared<BuiltinProcValue>(&builtin::eval)});
    builtin_map.insert({"exit", std::make_shared<BuiltinProcValue>(&builtin::exit)});
    builtin_map.insert({"newline", std::make_shared<BuiltinProcValue>(&builtin::newline)});
    builtin_map.insert({"print", std::make_shared<BuiltinProcValue>(&builtin::print)});
    //类型检查库 done
    builtin_map.insert({"atom?", std::make_shared<BuiltinProcValue>(&isT<&Value::isAtom>)});
    builtin_map.insert({"boolean?", std::make_shared<BuiltinProcValue>(&isT<&Value::isBoolean>)});
    builtin_map.insert({"integer?", std::make_shared<BuiltinProcValue>(&isT<&Value::isInteger>)});
    builtin_map.insert({"list?", std::make_shared<BuiltinProcValue>(&isT<&Value::isList>)});
    builtin_map.insert({"number?", std::make_shared<BuiltinProcValue>(&isT<&Value::isNumber>)});
    builtin_map.insert({"null?", std::make_shared<BuiltinProcValue>(&isT<&Value::isNil>)});
    builtin_map.insert({"pair?", std::make_shared<BuiltinProcValue>(&isT<&Value::isPair>)});
    builtin_map.insert({"procedure?", std::make_shared<BuiltinProcValue>(&isT<&Value::isProcedure>)});
    builtin_map.insert({"string?", std::make_shared<BuiltinProcValue>(&isT<&Value::isString>)});
    builtin_map.insert({"symbol?", std::make_shared<BuiltinProcValue>(&isT<&Value::isSymbol>)});
    //对子与列表操作库 done
    builtin_map.insert({"append", std::make_shared<BuiltinProcValue>(&builtin::append)});
    builtin_map.insert({"cdr", std::make_shared<BuiltinProcValue>(&builtin::cdr)});
    builtin_map.insert({"car", std::make_shared<BuiltinProcValue>(&builtin::car)});
    builtin_map.insert({"cons", std::make_shared<BuiltinProcValue>(&builtin::cons)});
    builtin_map.insert({"length", std::make_shared<BuiltinProcValue>(&builtin::length)});
    builtin_map.insert({"list", std::make_shared<BuiltinProcValue>(&builtin::list)});
    builtin_map.insert({"map", std::make_shared<BuiltinProcValue>(&builtin::map)});
    builtin_map.insert({"filter", std::make_shared<BuiltinProcValue>(&builtin::filter)});
    builtin_map.insert({"reduce", std::make_shared<BuiltinProcValue>(&builtin::reduce)});
    //算数运算库 done
    builtin_map.insert({"+", std::make_shared<BuiltinProcValue>(&builtin::add)});
    builtin_map.insert({"-", std::make_shared<BuiltinProcValue>(&builtin::minus)});
    builtin_map.insert({"*", std::make_shared<BuiltinProcValue>(&builtin::plus)});
    builtin_map.insert({"/", std::make_shared<BuiltinProcValue>(&builtin::divide)});
    builtin_map.insert({"abs", std::make_shared<BuiltinProcValue>(&builtin::abs)});
    builtin_map.insert({"expt", std::make_shared<BuiltinProcValue>(&builtin::expt)});
    builtin_map.insert({"quotient", std::make_shared<BuiltinProcValue>(&builtin::quotient)});
    builtin_map.insert({"remainder", std::make_shared<BuiltinProcValue>(&builtin::remainder)});
    builtin_map.insert({"modulo", std::make_shared<BuiltinProcValue>(&builtin::modulo)});
    //比较库 done
    builtin_map.insert({"eq?", std::make_shared<BuiltinProcValue>(&builtin::eq)});
    builtin_map.insert({"equal?", std::make_shared<BuiltinProcValue>(&builtin::equal)});
    builtin_map.insert({"not", std::make_shared<BuiltinProcValue>(&builtin::no)});
    builtin_map.insert({"=", std::make_shared<BuiltinProcValue>(&builtin::equ)});
    builtin_map.insert({"<", std::make_shared<BuiltinProcValue>(&builtin::less)});
    builtin_map.insert({">", std::make_shared<BuiltinProcValue>(&builtin::larger)});
    builtin_map.insert({"<=", std::make_shared<BuiltinProcValue>(&builtin::lesse)});
    builtin_map.insert({">=", std::make_shared<BuiltinProcValue>(&builtin::largere)});
    builtin_map.insert({"even?", std::make_shared<BuiltinProcValue>(&builtin::even)});
    builtin_map.insert({"odd?", std::make_shared<BuiltinProcValue>(&builtin::odd)});
    builtin_map.insert({"zero?", std::make_shared<BuiltinProcValue>(&builtin::zero)});
}

// 先写一些工具函数对代码进行一定的抽象 否则复制粘贴也挺麻烦的
PairValue* builtin::getPair(const std::vector<ValuePtr>& params) {
    CHECKONEPARAM(params);
    auto& param = params[0];
    if (!Value::isPair(param)) {
        throw LispError("Should get a pair.");
    }
    auto pair = dynamic_cast<PairValue*>(param.get());
    return pair;
}
std::vector<double> builtin::checkAndGetTwoNums(
    const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("The size of params is not correct.");
    }
    const auto& valx = params[0];
    const auto& valy = params[1];
    if (!valx->isNumber(valx) || (!valy->isNumber(valy))) {
        throw LispError("Cannot operate a non-numeric value.");
    }
    auto y = valy->asNumber(valy);
    auto x = valx->asNumber(valx);
    return {x, y};
}
double builtin::checkAndGetOneNum(const std::vector<ValuePtr>& params) {
    CHECKONEPARAM(params);
    auto& param = params[0];
    if (!param->isNumber(param)) {
        throw LispError("Cannot operate a non-numeric value.");
    }
    auto value = param->asNumber(param);
    return value;
}
int builtin::getOneInteger(const std::vector<ValuePtr>& params) {
    auto value = checkAndGetOneNum(params);
    if (value != static_cast<int>(value))
        throw LispError("Should get a integer.");
    int num = static_cast<int>(value);
    return num;
}
std::vector<ValuePtr> builtin::getProcAndList(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) throw LispError("Should be a proc and a list.");
    auto& proc = params[0];
    return {proc, params[1]};
}

// 核心库 done
ValuePtr builtin::apply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto paa = getProcAndList(params);
    ValuePtr proc{paa[0]}, args {paa[1]};
    return env.apply(proc,Value::toVec(args));
}
ValuePtr builtin::display(const std::vector<ValuePtr>& params, EvalEnv& env) {
    CHECKONEPARAM(params);
    auto& param = params[0];
    if (param->isString(param)) {
        std::cout << param->asString(param) << "\n";
        return std::make_shared<NilValue>();
    }
    std::cout << param->toString() << "\n";
    return std::make_shared<NilValue>();
}
ValuePtr builtin::displayln(const std::vector<ValuePtr>& params, EvalEnv& env) {
    display(params,env);
    newline({}, env);
    return std::make_shared<NilValue>();
}
ValuePtr builtin::error(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size()==1) {
        auto& param = params[0];
        throw LispError(param->toString());
    }
    if (params.size()==0) {
        throw LispError("Error.");
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::eval(const std::vector<ValuePtr>& params, EvalEnv& env) {
    CHECKONEPARAM(params);
    return env.eval(params[0]);
}
ValuePtr builtin::exit(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size()==0) {
        std::exit(0);
    }
    if (params.size()==1) {
        auto& param = params[0];
        std::exit(param->asNumber(param));
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::newline(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size()==0) {
        std::cout << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::print(const std::vector<ValuePtr>& params, EvalEnv& env) {
    CHECKONEPARAM(params);
    std::cout << params[0]->toString() << "\n";
    return std::make_shared<NilValue>();
}

//对子与列表操作库
ValuePtr builtin::cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return getPair(params)->get_cdr();
}
ValuePtr builtin::car(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return getPair(params)->get_car();
}
ValuePtr builtin::cons(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2) {
        throw LispError("The size of params is not correct.");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
}
ValuePtr builtin::length(const std::vector<ValuePtr>& params, EvalEnv& env) {
    CHECKONEPARAM(params);
    auto& param = params[0];
    auto vec = Value::toVec(param);
    return std::make_shared<NumericValue>(vec.size());
}
ValuePtr builtin::list(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto list = vec2pair(params);
    return list;
}
ValuePtr builtin::append(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::vector<ValuePtr>ans;
    for (auto& pair : params) {
        auto list = Value::toVec(pair);
        ans.append_range(list);
    } 
    return vec2pair(ans);
}
ValuePtr builtin::map(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto paa = getProcAndList(params);
    auto proc {paa[0]};
    auto list {Value::toVec(paa[1])};
    std::vector<ValuePtr>result;
    std::ranges::transform(list, std::back_inserter(result),
                           [&proc,&env](ValuePtr& v) { return env.apply(proc,{v}); });
    return vec2pair(result);
}
ValuePtr builtin::filter(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto paa = getProcAndList(params);
    auto proc{paa[0]};
    auto list{Value::toVec(paa[1])};
    std::vector<ValuePtr> result;
    for (auto& value:list){
        if (auto ans = env.apply(proc, {value});ans->asBoolean(ans)) {
            result.push_back(value);
        }
    }
    return vec2pair(result);
}
ValuePtr builtin::reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto paa = getProcAndList(params);
    auto proc{paa[0]};
    auto list{paa[1]};
    if (Value::isNil(list)) throw LispError("The list cannot be a Nil.");
    if (Value::toVec(list).size() == 1) 
        return car({list},env);
    auto result =  apply({proc, std::make_shared<PairValue>(car({list}, env),std::make_shared<PairValue>(reduce({proc, cdr({list},env)},env),std::make_shared<NilValue>()))},env);
    return result;
}
//算数运算库
ValuePtr builtin::add(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 0.0;
    for (auto& i : params) {
        if (!i->isNumber(i)) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber(i);
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::minus(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 0.0;
    double x,y;
    if (params.size() != 1 && params.size() != 2) {
        throw LispError("The size of params is not correct.");
    }
    if (params.size() == 1) {
        y = checkAndGetOneNum(params);
        x = 0.0;
    }
    if (params.size() == 2) {
        auto nums = checkAndGetTwoNums(params);
        y = nums[1];
        x = nums[0];
    }
    result = x-y;
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::plus(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber(i)) {
            throw LispError("Cannot plus a non-numeric value.");
        }
        result *= i->asNumber(i);
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::divide(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 0.0;
    double x, y;
    if (params.size() != 1 && params.size() != 2) {
        throw LispError("The size of params is not correct.");
    }
    if (params.size() == 1) {
        y = checkAndGetOneNum(params);
        x = 1.0;
    }
    if (params.size() == 2) {
        auto nums = checkAndGetTwoNums(params);
        y = nums[1];
        x = nums[0];
    }
    CHECKDIVIDEND(y);
    result = x / y;
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::abs(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto value = checkAndGetOneNum(params);
    value = std::abs(value);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::expt(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    if (x==0&&y==0){
        throw LispError("It is not defined.");
    }
    auto value = std::pow(x,y);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    CHECKDIVIDEND(y);
    auto value = y/x;
    value = std::trunc(value);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    CHECKDIVIDEND(y);
    double q = x - y * std::floor(x / y);
    return std::make_shared<NumericValue>(q);
}
ValuePtr builtin::remainder(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    CHECKDIVIDEND(y);
    auto t = std::trunc(x/y);
    auto q =  x - y * t;
    return std::make_shared<NumericValue>(q);
}

//比较库
ValuePtr builtin::eq(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2) throw LispError("Should get two params.");
    auto first{params[0]}, second {params[1]};
    if (typeid(*first)!=typeid(*second))
        return std::make_shared<BooleanValue>(false);
    if (first == second) return std::make_shared<BooleanValue>(true);
    if (Value::isNumber(first)){
        return std::make_shared<BooleanValue>(first->asNumber(first)==second->asNumber(second));
    }
    else if (Value::isBoolean(first)) {
        return std::make_shared<BooleanValue>(first->asBoolean(first) ==second->asBoolean(second));
    } else if (Value::isSymbol(first)) {
        return std::make_shared<BooleanValue>(first->asSymbol(first) ==second->asSymbol(second));
    } else if (Value::isNil(first)) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}
ValuePtr builtin::equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (eq(params,env)->asBoolean(eq(params,env))) return std::make_shared<BooleanValue>(true);
    auto first{params[0]}, second{params[1]};
    if (typeid(*first) != typeid(*second))
        return std::make_shared<BooleanValue>(false);
    if (Value::isString(first)){
        return std::make_shared<BooleanValue>(first->asString(first)==second->asString(second));
    }
    else if (Value::isPair(first)) {
        if (Value::isList(first) != Value::isList(second))
            return std::make_shared<BooleanValue>(false);
        auto listA = Value::toVec(first);
        auto listB = Value::toVec(second);
        if (listA.size() != listB.size())
            return std::make_shared<BooleanValue>(false);
        auto pairA = dynamic_cast<PairValue*>(first.get());
        auto pairB = dynamic_cast<PairValue*>(second.get());
        auto eqleft = equal({pairA->get_car(), pairB->get_car()},env);
        auto eqright = equal({pairA->get_cdr(),pairB->get_cdr()},env);
        return std::make_shared<BooleanValue>(eqleft->asBoolean(eqleft)&&eqright->asBoolean(eqright));
    } else if (Value::isProcedure(first)) {
        throw LispError("The compare of procedures is not defined.");
    }
    return std::make_shared<BooleanValue>(false);
}
ValuePtr builtin::no(const std::vector<ValuePtr>& params, EvalEnv& env) {
    CHECKONEPARAM(params);
    auto& value = params[0];
    return std::make_shared<BooleanValue> (!value->asBoolean(value));
}
ValuePtr builtin::equ(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y {nums[1]};
    return std::make_shared<BooleanValue>(x==y);
}
ValuePtr builtin::less(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x < y);
}
ValuePtr builtin::larger(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x > y);
}
ValuePtr builtin::lesse(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x <= y);
}
ValuePtr builtin::largere(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x >= y);
}
ValuePtr builtin::even(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(getOneInteger(params) % 2 == 0);
}
ValuePtr builtin::odd(const std::vector<ValuePtr>& params, EvalEnv& env) {
    return std::make_shared<BooleanValue>(getOneInteger(params) % 2 != 0);
}
ValuePtr builtin::zero(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto value = checkAndGetOneNum(params);
    return std::make_shared<BooleanValue>(value==0);
}
