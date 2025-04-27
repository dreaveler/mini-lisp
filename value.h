#pragma once
#ifndef VALUE_H
#define VALUE_H
#include<string>
#include<memory>
class Value {
public:
    virtual ~Value() {};
    virtual std::string toString() const;
    virtual bool isSelfEvaluating() const;
    virtual bool isNil() const;
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value {
private:
    const bool value;

public:
    BooleanValue(bool v) : Value(), value{v} {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};
class NumericValue :public Value {
private:
    const double value;

public:
    NumericValue(double v) : Value(), value(v) {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};
class StringValue : public Value {
private:
    const std::string value;

public:
    StringValue(std::string v) : Value(), value{v} {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};
class NilValue : public Value {
public:
    NilValue():Value() {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};
class SymbolValue : public Value {
private:
    const std::string name;

public:
    SymbolValue(std::string n) : Value(), name(n) {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};
class PairValue : public Value {
private:
    const ValuePtr left;
    const ValuePtr right;

public:
    PairValue(ValuePtr l, ValuePtr r)
        : Value(), left{l}, right{r} {};
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
};

#endif
