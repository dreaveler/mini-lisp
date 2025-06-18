#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include "value.h"
#include<vector>
inline ValuePtr vec2pair(std::vector<ValuePtr> v) {
    if (v.empty()) {
        return std::make_shared<NilValue>();
    }
    return std::make_shared<PairValue>(
        v[0], vec2pair(std::vector<ValuePtr>(v.begin() + 1, v.end())));
}
inline std::vector<ValuePtr> toVec(const ValuePtr& v) {
    std::vector<ValuePtr> ans;
    if (typeid(*v) == typeid(NilValue)) {
        return ans;
    }
    auto pair = dynamic_cast<PairValue*>(v.get());
    auto current = pair->get_cdr();
    ans.push_back(pair->get_car());
    while (true) {
        if (dynamic_cast<NilValue*>(current.get())) {
            return ans;
        }
        if (auto next = dynamic_cast<PairValue*>(current.get())) {
            ans.push_back(next->get_car());
            current = next->get_cdr();
        }
    }
    throw SyntaxError("The list is not right.");
}

#endif
