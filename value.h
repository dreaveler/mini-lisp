#pragma once
#ifndef VALUE_H
#define VALUE_H
#include<string>
#include<memory>
#include<optional>
#include<vector>
class Value;
using ValuePtr = std::shared_ptr<Value>;

class Value {
public:
    virtual ~Value() {};
    virtual std::string toString() const;
    static bool isSelfEvaluating(const ValuePtr& v);
    static bool isNil(const ValuePtr& v);
    static bool isSymbol(const ValuePtr& v);
    static bool isList(const ValuePtr& v);
    static std::vector<ValuePtr> toVec(const ValuePtr& v);
    static std::optional<std::string> asSymbol(const ValuePtr& v);
};

class BooleanValue : public Value {
private:
    const bool value;

public:
    BooleanValue(bool v) : Value(), value{v} {};
    std::string toString() const override;
};
class NumericValue :public Value {
private:
    const double value;

public:
    NumericValue(double v) : Value(), value(v) {};
    std::string toString() const override;
};
class StringValue : public Value {
private:
    const std::string value;

public:
    StringValue(std::string v) : Value(), value{v} {};
    std::string toString() const override;
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
    std::string get_name() const;
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

#endif
