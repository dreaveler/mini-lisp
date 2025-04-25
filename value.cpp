#include"value.h"
#include<iomanip>
#include<sstream>
std::string Value::toString() const {
    return "";
}
std::string BooleanValue::toString() const {
    std::string str{"#"};
    if (value == true)
        str += "t";
    else
        str += "f";
    return str;
}
std::string NumericValue::toString() const {
    if (value == static_cast<int>(value)) {
        int v{static_cast<int>(value)};
        return std::to_string(v);
    } 
    else
        return std::to_string(value);
}
std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}
std::string NilValue::toString() const {
    return "()";
}
std::string SymbolValue::toString() const {
    return name;
}
std::string PairValue::toString() const {
    std::string str{"("};
    ValuePtr rn{right};
    ValuePtr ln{left};
    while (typeid(*rn) == typeid(PairValue)) {
        str += ln->toString();
        str += " ";
        auto& pair = static_cast<const PairValue&>(*rn);
        rn = pair.right;
        ln = pair.left;
    }
    if (typeid(*rn) == typeid(NilValue)) {
        str += ln->toString();
        str += ")";
    }
    else {
        str += ln->toString();
        str += " . ";
        str += rn->toString();
        str += ")";
    }
    return str;
}
