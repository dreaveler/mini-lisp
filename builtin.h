#pragma once
#ifndef BUILTIN_H
#define BUILTIN_H
#include"value.h"
#include<unordered_map>

class Builtin {
public:
    std::unordered_map<std::string, ValuePtr> builtin_map;
    Builtin();
};

namespace builtin {
    bool isOneParam(const std::vector<ValuePtr>& params);
    bool isZeroParam(const std::vector<ValuePtr>& params);
    ValuePtr add(const std::vector<ValuePtr>& params);
    //核心库
    ValuePtr print(const std::vector<ValuePtr>& params);
    ValuePtr display(const std::vector<ValuePtr>& params);
    ValuePtr error(const std::vector<ValuePtr>& params);
    ValuePtr exit(const std::vector<ValuePtr>& params);
    ValuePtr newline(const std::vector<ValuePtr>& params);
    //类型检查库
    ValuePtr isAtom(const std::vector<ValuePtr>& params);
    ValuePtr isBoolean(const std::vector<ValuePtr>& params);
    ValuePtr isInteger(const std::vector<ValuePtr>& params);
    ValuePtr isList(const std::vector<ValuePtr>& params);
    ValuePtr isNumber(const std::vector<ValuePtr>& params);
    ValuePtr isNull(const std::vector<ValuePtr>& params);
    ValuePtr isPair(const std::vector<ValuePtr>& params);
    ValuePtr isProcedure(const std::vector<ValuePtr>& params);
    ValuePtr isString(const std::vector<ValuePtr>& params);
    ValuePtr isSymbol(const std::vector<ValuePtr>& params);
    }

#endif  
