#include"builtin.h"
#include"error.h"
#include<iostream>

Builtin::Builtin() {
    // 核心库
    builtin_map.insert({"+", std::make_shared<BuiltinProcValue>(&builtin::add)});
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
}

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
//工具
bool builtin::isOneParam(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        return false;
    }
    return true;
}
bool builtin::isZeroParam(const std::vector<ValuePtr>& params) {
    if (params.size() != 0) {
        return false;
    }
    return true;
}
// 核心库
ValuePtr builtin::print(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        std::cout << params[0]->toString() << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::display(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
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
    if (isOneParam(params)) {
        auto& param = params[0];
        throw LispError(param->toString());
    }
    if (isZeroParam(params)) {
        throw LispError("Error.");
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::exit(const std::vector<ValuePtr>& params) {
    if (isZeroParam(params)) {
        std::exit(0);
    }
    if (isOneParam(params)) {
        auto& param = params[0];
        std::exit(param->asNumber(param));
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::newline(const std::vector<ValuePtr>& params) {
    if (isZeroParam(params)) {
        std::cout << "\n";
        return std::make_shared<NilValue>();
    }
    throw LispError("The size of params is not correct.");
}
//类型检查库
ValuePtr builtin::isAtom(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)){
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
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isNil(param)) {
            return std::make_shared<BooleanValue>(true);
        } else 
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isInteger(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
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
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isList(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
//上面这个需要修改  须发判定对子列表不是列表
ValuePtr builtin::isNumber(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isNumber(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isNull(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isNil(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isPair(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isPair(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isProcedure(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isBuiltin(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isString(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isString(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
ValuePtr builtin::isSymbol(const std::vector<ValuePtr>& params) {
    if (isOneParam(params)) {
        auto& param = params[0];
        if (param->isSymbol(param)) {
            return std::make_shared<BooleanValue>(true);
        } else
            return std::make_shared<BooleanValue>(false);
    }
    throw LispError("The size of params is not correct.");
}
