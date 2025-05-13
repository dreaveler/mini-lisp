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


#endif
