#pragma once
#ifndef VALUE_H
#define VALUE_H
#include<string>
#include<memory>
#include<optional>
#include<vector>
#include<functional>
#include<iomanip>
#include<sstream>
#include"error.h"
class Value;
using ValuePtr = std::shared_ptr<Value>;

class EvalEnv;

class Value {
public:
    virtual ~Value() {};
    //toString用于输出
    virtual std::string toString() const;
    //bool函数用于判断  且可在内置过程中被调用 可能需要重构一下
    static bool isSelfEvaluating(const ValuePtr& v);
    static bool isAtom(const ValuePtr& v);
    static bool isBoolean(const ValuePtr& v);
    static bool isNil(const ValuePtr& v);
    static bool isSymbol(const ValuePtr& v);
    static bool isList(const ValuePtr& v);
    static bool isNumber(const ValuePtr& v);
    static bool isInteger(const ValuePtr& v);
    static bool isString(const ValuePtr& v);
    static bool isPair(const ValuePtr& v);
    static bool isLambda(const ValuePtr& v);
    static bool isBuiltin(const ValuePtr& v);
    static bool isProcedure(const ValuePtr& v);

    //as函数返回需要的值
    virtual std::optional<std::string> asSymbol(const ValuePtr& v);
    virtual double asNumber(const ValuePtr& v) const;
    virtual std::string asString(const ValuePtr& v) const;
    virtual bool asBoolean(const ValuePtr& v) const;
};

class BooleanValue : public Value {
private:
    const bool value;

public:
    BooleanValue(bool v) : Value(), value{v} {};
    std::string toString() const override;
    bool asBoolean(const ValuePtr& v) const override;
};
class NumericValue :public Value {
private:
    const double value;
    bool integer;   //true即为是整数  false即不是整数  在生成value时判断并保存为属性
public:
    NumericValue(double v)
        : Value(), value(v), integer(v == static_cast<int>(v)) {};
    std::string toString() const override;
    double asNumber(const ValuePtr& v) const override;
    bool IsInteger() const;
};
class StringValue : public Value {
private:
    const std::string value;

public:
    StringValue(std::string v) : Value(), value{v} {};
    std::string toString() const override;
    std::string asString(const ValuePtr& v) const override;
};
class NilValue : public Value {
public:
    NilValue():Value() {};
    std::string toString() const override;
};
class SymbolValue : public Value {
private:
    const std::string name;

public:
    SymbolValue(std::string n) : Value(), name(n) {};
    std::string toString() const override;
    std::optional<std::string> asSymbol(const ValuePtr& v) override;
};
class PairValue : public Value {
private:
    const ValuePtr left;
    const ValuePtr right;

public:
    PairValue(ValuePtr l, ValuePtr r)
        : Value(), left{l}, right{r} {};
    std::string toString() const override;
    ValuePtr get_cdr() const;
    ValuePtr get_car() const;
};

using BultinFuncType = ValuePtr(const std::vector<ValuePtr>&,EvalEnv&);

class BuiltinProcValue : public Value {  
private:  
   BultinFuncType* func;  
public:  
   BuiltinProcValue(BultinFuncType* f) : Value(), func(f) {};
   std::string toString() const override;  
   BultinFuncType* getFunc() const;
   bool isTypeCheck() const;
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> parent;
public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body ,std::shared_ptr<EvalEnv> env)
        : params(params), body(body) ,parent(env) {};
    std::string toString() const override;  // 如前所述，返回 #<procedure> 即可
    ValuePtr apply(const std::vector<ValuePtr>args) const;

};

#endif
