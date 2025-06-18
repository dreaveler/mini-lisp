#pragma once
#ifndef IO_H
#define IO_H
#include<string>
#include"eval_env.h"
#include<iostream>
//使用模板设计模式完成io
class Input {
protected:
    std::istream* in;
public:
    Input(std::istream* in) : in(in) {};
    virtual ~Input();
    std::string Lisp(std::shared_ptr<EvalEnv> env, std::string line);
    void processInput(std::shared_ptr<EvalEnv> env);
    virtual void processOne() = 0;
    virtual void processTwo(std::string result) = 0;
    static Input* parseArgs(int argc, char** argv);
};

class ReplInput : public Input {
public:
    ~ReplInput();
    ReplInput(std::istream* in) : Input(in) {};
    void processOne();
    void processTwo(std::string result);
};
class FileInput : public Input {
public:
    ~FileInput();
    FileInput(std::istream* in) : Input(in) {};
    void processOne();
    void processTwo(std::string result);
};
#endif
