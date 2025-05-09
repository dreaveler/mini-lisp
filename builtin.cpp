#include"builtin.h"
#include"error.h"
#include<iostream>
#include"parser.h"
#include<cmath>

Builtin::Builtin() {
    // 核心库
    builtin_map.insert({"print", std::make_shared<BuiltinProcValue>(&builtin::print)});
    builtin_map.insert({"display", std::make_shared<BuiltinProcValue>(&builtin::display)});
    builtin_map.insert({"error", std::make_shared<BuiltinProcValue>(&builtin::error)});
    builtin_map.insert({"exit", std::make_shared<BuiltinProcValue>(&builtin::exit)});
    builtin_map.insert({"newline", std::make_shared<BuiltinProcValue>(&builtin::newline)});
    //类型检查库
    builtin_map.insert({"atom?", std::make_shared<BuiltinProcValue>(&builtin::isAtom)});
    builtin_map.insert({"boolean?", std::make_shared<BuiltinProcValue>(&builtin::isBoolean)});
    builtin_map.insert({"integer?", std::make_shared<BuiltinProcValue>(&builtin::isInteger)});
    builtin_map.insert({"list?", std::make_shared<BuiltinProcValue>(&builtin::isList)});
    builtin_map.insert({"number?", std::make_shared<BuiltinProcValue>(&builtin::isNumber)});
    builtin_map.insert({"null?", std::make_shared<BuiltinProcValue>(&builtin::isNull)});
    builtin_map.insert({"pair?", std::make_shared<BuiltinProcValue>(&builtin::isPair)});
    builtin_map.insert({"procedure?", std::make_shared<BuiltinProcValue>(&builtin::isProcedure)});
    builtin_map.insert({"string?", std::make_shared<BuiltinProcValue>(&builtin::isString)});
    builtin_map.insert({"symbol?", std::make_shared<BuiltinProcValue>(&builtin::isSymbol)});
    //对子与列表操作库
    builtin_map.insert({"cdr", std::make_shared<BuiltinProcValue>(&builtin::cdr)});
    builtin_map.insert({"car", std::make_shared<BuiltinProcValue>(&builtin::car)});
    builtin_map.insert({"cons", std::make_shared<BuiltinProcValue>(&builtin::cons)});
    builtin_map.insert({"length", std::make_shared<BuiltinProcValue>(&builtin::length)});
    builtin_map.insert({"list", std::make_shared<BuiltinProcValue>(&builtin::list)});
    builtin_map.insert({"append", std::make_shared<BuiltinProcValue>(&builtin::append)});
    //算数运算库
    builtin_map.insert({"+", std::make_shared<BuiltinProcValue>(&builtin::add)});
    builtin_map.insert({"-", std::make_shared<BuiltinProcValue>(&builtin::minus)});
    builtin_map.insert({"*", std::make_shared<BuiltinProcValue>(&builtin::plus)});
    builtin_map.insert({"/", std::make_shared<BuiltinProcValue>(&builtin::divide)});
    builtin_map.insert({"abs", std::make_shared<BuiltinProcValue>(&builtin::abs)});
    builtin_map.insert({"expt", std::make_shared<BuiltinProcValue>(&builtin::expt)});
    builtin_map.insert({"quotient", std::make_shared<BuiltinProcValue>(&builtin::quotient)});
    builtin_map.insert({"remainder", std::make_shared<BuiltinProcValue>(&builtin::remainder)});
    builtin_map.insert({"modulo", std::make_shared<BuiltinProcValue>(&builtin::modulo)});
    //比较库
    builtin_map.insert({"=", std::make_shared<BuiltinProcValue>(&builtin::equ)});
    builtin_map.insert({"<", std::make_shared<BuiltinProcValue>(&builtin::less)});
    builtin_map.insert({">", std::make_shared<BuiltinProcValue>(&builtin::larger)});
    builtin_map.insert({"<=", std::make_shared<BuiltinProcValue>(&builtin::lesse)});
    builtin_map.insert({">=", std::make_shared<BuiltinProcValue>(&builtin::largere)});
    builtin_map.insert({"even?", std::make_shared<BuiltinProcValue>(&builtin::even)});
    builtin_map.insert({"odd?", std::make_shared<BuiltinProcValue>(&builtin::odd)});
    builtin_map.insert({"zero?", std::make_shared<BuiltinProcValue>(&builtin::zero)});
}


// 核心库
ValuePtr builtin::print(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        std::cout << params[0]->toString() << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::display(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isString(param)) {
            std::cout << param->asString(param) << "\n";
            return std::make_shared<NilValue>();
        }
        std::cout << param->toString() << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::error(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        throw LispError(param->toString());
    }
    if (params.size()==0) {
        throw LispError("Error.");
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::exit(const std::vector<ValuePtr>& params) {
    if (params.size()==0) {
        std::exit(0);
    }
    if (params.size()==1) {
        auto& param = params[0];
        std::exit(param->asNumber(param));
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::newline(const std::vector<ValuePtr>& params) {
    if (params.size()==0) {
        std::cout << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
//类型检查库
ValuePtr builtin::isAtom(const std::vector<ValuePtr>& params) {
    if (params.size()==1){
        auto& param = params[0];
        if (param->isSelfEvaluating(param) || param->isNil(param) ||
            param->isSymbol(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isBoolean(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isNil(param)) {
            return std::make_shared<BooleanValue>(true);
        } else 
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isInteger(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (!param->isNumber(param)) {
            return std::make_shared<BooleanValue>(false);
        } 
        auto num = param->asNumber(param);
        auto intnum = static_cast<int>(num);
        if (num != double(intnum)) {
            return std::make_shared<BooleanValue>(false);
        }
        return std::make_shared<BooleanValue>(true);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isList(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isList(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isNumber(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isNumber(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isNull(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isNil(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isPair(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isPair(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isProcedure(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isBuiltin(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isString(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isString(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isSymbol(const std::vector<ValuePtr>& params) {
    if (params.size()==1) {
        auto& param = params[0];
        if (param->isSymbol(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
//对子与列表操作库
ValuePtr builtin::cdr(const std::vector<ValuePtr>& params) {
    if (params.size()!=1) {
        throw LispError("The size of params is not correct.");
    } 
    auto param = params[0];
    if (!param->isPair(param)){
        throw LispError("Should get a pair.");
    }
    auto pair = dynamic_cast<PairValue*>(param.get());
    return pair->get_cdr();
}
ValuePtr builtin::car(const std::vector<ValuePtr>& params) {
    if (params.size()!=1) {
        throw LispError("The size of params is not correct.");
    }
    auto param = params[0];
    if (!param->isPair(param)) {
        throw LispError("Should get a pair.");
    }
    auto pair = dynamic_cast<PairValue*>(param.get());
    return pair->get_car();
}
ValuePtr builtin::cons(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("The size of params is not correct.");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
}
ValuePtr builtin::length(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("The size of params is not correct.");
    }
    auto param = params[0];
    auto vec = param->toVec(param);
    return std::make_shared<NumericValue>(vec.size());
}
ValuePtr builtin::list(const std::vector<ValuePtr>& params) {
    auto list = parser::vec2pair(params);
    return list;
}
ValuePtr builtin::append(const std::vector<ValuePtr>& params) {
    std::vector<ValuePtr>ans;
    for (auto& pair : params) {
        auto list = pair->toVec(pair);
        ans.append_range(list);
    } 
    return parser::vec2pair(ans);
}
//算数运算库
ValuePtr builtin::add(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    for (const auto& i : params) {
        if (!i->isNumber(i)) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber(i);
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::minus(const std::vector<ValuePtr>& params) {
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
ValuePtr builtin::plus(const std::vector<ValuePtr>& params) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber(i)) {
            throw LispError("Cannot plus a non-numeric value.");
        }
        result *= i->asNumber(i);
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::divide(const std::vector<ValuePtr>& params) {
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
    if (y == 0) {
        throw LispError("Cannot divide a zero value.");
    }
    result = x / y;
    return std::make_shared<NumericValue>(result);
}
ValuePtr builtin::abs(const std::vector<ValuePtr>& params) {
    auto value = checkAndGetOneNum(params);
    value = std::abs(value);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::expt(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    if (x==0&&y==0){
        throw LispError("It is not defined.");
    }
    auto value = std::pow(x,y);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::quotient(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    if (y == 0) throw LispError("Cannot divide a zero value.");
    auto value = y/x;
    value = std::trunc(value);
    return std::make_shared<NumericValue>(value);
}
ValuePtr builtin::modulo(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    if (y == 0) throw LispError("Cannot divide a zero value.");
    double q = x - y * std::floor(x / y);
    return std::make_shared<NumericValue>(q);
}
ValuePtr builtin::remainder(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    auto y = nums[1];
    auto x = nums[0];
    if (y == 0) throw LispError("Cannot divide a zero value.");
    auto t = std::trunc(x/y);
    auto q =  x - y * t;
    return std::make_shared<NumericValue>(q);
}
//先写一些工具函数对代码进行一定的抽象 否则复制粘贴也挺麻烦的
std::vector<double> builtin::checkAndGetTwoNums(const std::vector<ValuePtr>& params) {
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
    return {x,y};
}
double builtin::checkAndGetOneNum(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("The size of params is not correct.");
    }
    auto param = params[0];
    if (!param->isNumber(param)) {
        throw LispError("Cannot operate a non-numeric value.");
    }
    auto value = param->asNumber(param);
    return value;
}
//比较库
ValuePtr builtin::equ(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y {nums[1]};
    return std::make_shared<BooleanValue>(x==y);
}
ValuePtr builtin::less(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x < y);
}
ValuePtr builtin::larger(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x > y);
}
ValuePtr builtin::lesse(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x <= y);
}
ValuePtr builtin::largere(const std::vector<ValuePtr>& params) {
    auto nums = checkAndGetTwoNums(params);
    double x{nums[0]}, y{nums[1]};
    return std::make_shared<BooleanValue>(x >= y);
}
ValuePtr builtin::even(const std::vector<ValuePtr>& params) {
    auto value = checkAndGetOneNum(params);
    if (value != static_cast<int>(value))
        throw LispError("Should get a integer.");
    int num = static_cast<int>(value);
    return std::make_shared<BooleanValue>(num%2==0);
}
ValuePtr builtin::odd(const std::vector<ValuePtr>& params) {
    auto value = checkAndGetOneNum(params);
    if (value != static_cast<int>(value))
        throw LispError("Should get a integer.");
    int num = static_cast<int>(value);
    return std::make_shared<BooleanValue>(num % 2 != 0);
}
ValuePtr builtin::zero(const std::vector<ValuePtr>& params) {
    auto value = checkAndGetOneNum(params);
    return std::make_shared<BooleanValue>(value==0);
}
