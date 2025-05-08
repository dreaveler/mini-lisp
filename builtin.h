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
void checkZeroOrOneParam(const std::vector<ValuePtr>& params);
ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr print(const std::vector<ValuePtr>& params);
ValuePtr display(const std::vector<ValuePtr>& params);
ValuePtr error(const std::vector<ValuePtr>& params);
ValuePtr exit(const std::vector<ValuePtr>& params);
ValuePtr newline(const std::vector<ValuePtr>& params);
}

#endif  
