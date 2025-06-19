#pragma once
#ifndef IO_H
#define IO_H
#include<string>
#include"eval_env.h"
#include<iostream>
//使用模板设计模式完成io
//这里没有用虚析构函数的原因是内容全部委派给智能指针了所以析构函数内无内容
class Inputer {
protected:
    std::unique_ptr<std::istream> in;
public:
    Inputer(std::istream* in) : in(in) {};
    void processInput(std::shared_ptr<EvalEnv> env);
    virtual void processOne() = 0;
    virtual void processTwo(std::string result) = 0;
    static std::unique_ptr<Inputer> parseArgs(int argc, char** argv);
};

class ReplInputer : public Inputer {
public:
    ReplInputer(std::istream* in) : Inputer(in) {};
    void processOne();
    void processTwo(std::string result);
};
class FileInputer : public Inputer {
public:
    FileInputer(std::istream* in) : Inputer(in) {};
    void processOne();
    void processTwo(std::string result);
};
#endif
