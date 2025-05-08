#include"builtin.h"
#include"error.h"
#include<iostream>

Builtin::Builtin() {
    builtin_map.insert({"+", std::make_shared<BuiltinProcValue>(&builtin::add)});
    builtin_map.insert({"print", std::make_shared<BuiltinProcValue>(&builtin::print)});
    builtin_map.insert({"display", std::make_shared<BuiltinProcValue>(&builtin::display)});
    builtin_map.insert({"error", std::make_shared<BuiltinProcValue>(&builtin::error)});
    builtin_map.insert({"exit", std::make_shared<BuiltinProcValue>(&builtin::exit)});
    builtin_map.insert({"newline", std::make_shared<BuiltinProcValue>(&builtin::newline)});
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

void builtin::checkZeroOrOneParam(const std::vector<ValuePtr>& params) {
    if (params.size() > 1) {
        throw LispError("Shoule get one param");
    }
}
ValuePtr builtin::print(const std::vector<ValuePtr>& params) {
    checkZeroOrOneParam(params);
    std::cout << params[0]->toString()<< "\n";
    return std::make_shared<NilValue>();
}
ValuePtr builtin::display(const std::vector<ValuePtr>& params) {
    checkZeroOrOneParam(params);
    auto param = params[0];
    if (param->isString(param)){
        std::cout << param->asString(param) << "\n";
        return std::make_shared<NilValue>();
    }
    std::cout << param->toString() << "\n";
    return std::make_shared<NilValue>();
}
ValuePtr builtin::error(const std::vector<ValuePtr>& params) {
    checkZeroOrOneParam(params);
    if (params.size() == 0) {
        throw LispError("Error.");
    }
    auto param = params[0];
    throw LispError(param->toString());
}
ValuePtr builtin::exit(const std::vector<ValuePtr>& params) {
    checkZeroOrOneParam(params);
    if (params.size() == 0) {
        std::exit(0);
    }
    auto param = params[0];
    std::exit(param->asNumber(param));
}
ValuePtr builtin::newline(const std::vector<ValuePtr>& params) {
    std::cout << "\n";
    return std::make_shared<NilValue>();
}

//其它的还在写  先把这个交上去  不要急
