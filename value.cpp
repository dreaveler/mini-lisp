#include"value.h"
#include<iomanip>
#include"error.h"
#include"builtin.h"
#include"eval_env.h"
#include<algorithm>
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
    if (isBoolean(v) ||
        isNumber(v)|| 
        isString(v)) {
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
bool Value::isBoolean(const ValuePtr& v) {
    if (typeid(*v) == typeid(BooleanValue)) {
        return true;
    }else return false;
}
bool Value::isAtom(const ValuePtr& v) {
    if (isSelfEvaluating(v)||
        isNil(v)|| isSymbol(v)) {
        return true;
    }else return false;
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
    else if (isNil(v)) {
        return true;
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
bool Value::isLambda(const ValuePtr& v) {
    if (typeid(*v) == typeid(LambdaValue)) {
        return true;
    }
    return false;
}
bool Value::isProcedure(const ValuePtr& v) {
    if (isBuiltin(v) || isLambda(v)) {
        return true;
    }else return false;
}
bool Value::isInteger(const ValuePtr& v) {
    if (!isNumber(v)) {
        return false;
    }
    auto ptr = dynamic_cast<NumericValue*>(v.get());
    return ptr->IsInteger();
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
bool NumericValue::IsInteger() const {
    return integer;
}
std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}

BultinFuncType* BuiltinProcValue::getFunc() const {
    return func;
}

bool BuiltinProcValue::isTypeCheck() const {
    if (func==isT<Value::isAtom>||
        func==isT<Value::isBoolean>||
        func==isT<Value::isInteger>||
        func==isT<Value::isList>||
        func==isT<Value::isNil>||
        func==isT<Value::isNumber>||
        func==isT<Value::isPair>||
        func==isT<Value::isProcedure>||
        func==isT<Value::isString>||
        func==isT<Value::isSymbol>){
            return true;
        }
    return false;
}

bool Value::asBoolean(const ValuePtr& v) const {
    if (auto boolean = dynamic_cast<BooleanValue*>(v.get())) {
        return boolean->asBoolean(v);
    }
    return true;
}
bool BooleanValue::asBoolean(const ValuePtr& v) const {
    return value;
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}
ValuePtr LambdaValue::apply(const std::vector<ValuePtr> args) const {
    auto current = parent->createChild(params,args);
    std::vector<ValuePtr> result;
    std::ranges::transform(body, std::back_inserter(result),
                           [current](ValuePtr v) { return current->eval(v); });
    return result[result.size()-1];
}
