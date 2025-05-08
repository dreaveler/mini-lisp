#include"value.h"
#include<iomanip>
#include<sstream>
#include"error.h"
#include"builtin.h"
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
    while (auto pair = dynamic_cast<const PairValue*>(rn.get())) {
        str += ln->toString();
        str += " ";
        rn = pair->right;
        ln = pair->left;
    }
    if (auto pair = dynamic_cast<NilValue*>(rn.get())) {
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
ValuePtr PairValue::get_cdr() const {
    return right;
}
ValuePtr PairValue::get_car() const {
    return left;
}

std::string StringValue::asString(const ValuePtr& V) const {
    return value;
}
std::string Value::asString(const ValuePtr& v) const {
    if (auto string = dynamic_cast<StringValue*>(v.get())) {
        return string->asString(v);
    }
    throw LispError("Should be a string.");
}

bool Value::isSelfEvaluating(const ValuePtr& v) {
    if (typeid(*v) == typeid(BooleanValue) ||
        typeid(*v) == typeid(NumericValue) || 
        typeid(*v) == typeid(StringValue)) {
    
    return true;
    } else {
        return false;
    }
}
bool Value::isNil(const ValuePtr& v) {
    if (typeid(*v)==typeid(NilValue)){
        return true;
    }
    else return false;
}
bool Value::isNumber(const ValuePtr& v) {
    if (typeid(*v) == typeid(NumericValue)) {
        return true;
    }else return false;
}
bool Value::isSymbol(const ValuePtr& v) {
    if (typeid(*v)==typeid(SymbolValue)){
        return true;
    }
    else return false;
}
bool Value::isList(const ValuePtr& v) {
    if (auto pair = dynamic_cast<PairValue*>(v.get())) {
        auto current = pair->get_cdr();
        while (true) {
            if (dynamic_cast<NilValue*>(current.get())) {
                return true;
            }
            if (auto next = dynamic_cast<PairValue*>(current.get())) {
                current = next->get_cdr();
            } else {
            return false;
            }
        }
    }
    else return false;
}
bool Value::isBuiltin(const ValuePtr& v){
    if (typeid(*v) == typeid(BuiltinProcValue)){
        return true;
    }
    return false;
}
bool Value::isString(const ValuePtr& v) {
    if (typeid(*v) == typeid(StringValue)) {
        return true;    
    }
    return false;
}
bool Value::isPair(const ValuePtr& v) {
    if (typeid(*v) == typeid(PairValue)) {
        return true;
    } 
    return false;
}

std::vector<ValuePtr> Value::toVec(const ValuePtr& v) {
    std::vector<ValuePtr>ans;
    if (typeid(*v) == typeid(NilValue)) {
        return ans;
    }
    auto pair = dynamic_cast<PairValue*>(v.get());
    auto current = pair->get_cdr();
    ans.push_back(pair->get_car());
    while (true){
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
std::optional<std::string> Value::asSymbol(const ValuePtr& v) {
    if (auto symbol = dynamic_cast<SymbolValue*>(v.get())) {
        return symbol->asSymbol(v);
    }
    else return std::nullopt;
}
std::optional<std::string> SymbolValue::asSymbol(const ValuePtr& v) {
    return std::optional<std::string>(name);
}
double Value::asNumber(const ValuePtr& v) const {
    if (auto number = dynamic_cast<NumericValue*>(v.get())){
        return number->asNumber(v);
    }
    throw SyntaxError("Should get a number.");
}
double NumericValue::asNumber(const ValuePtr& v) const {
    return value;
}
std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}

BultinFuncType* BuiltinProcValue::getFunc() const {
    return func;
}

bool BuiltinProcValue::isTypeCheck() const {
    if (func==builtin::isAtom||
        func==builtin::isBoolean||
        func==builtin::isInteger||
        func==builtin::isList||
        func==builtin::isNull||
        func==builtin::isNumber||
        func==builtin::isPair||
        func==builtin::isProcedure||
        func==builtin::isString||
        func==builtin::isSymbol){
            return true;
        }
    return false;
}
