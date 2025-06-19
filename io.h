#pragma once
#ifndef IO_H
#define IO_H
#include<string>
#include"eval_env.h"
#include<iostream>
#include<stack>
#include"token.h"
//使用模板设计模式完成io
class Input {
protected:
    std::istream* in;
public:
    Input(std::istream* in) : in(in) {};
    virtual ~Input();
    void processInput(std::shared_ptr<EvalEnv> env);
    virtual void processOneFirstLine() = 0;
    virtual void processOneOtherLine() = 0;
    virtual void processTwo(std::string result) = 0;
    static std::unique_ptr<Input> parseArgs(int argc, char** argv);
    bool checkParen(const std::deque<TokenPtr>& tokens,
                    std::stack<char> parenContainer);
};

class ReplInput : public Input {
public:
    ~ReplInput();
    ReplInput(std::istream* in) : Input(in) {};
    void processOneFirstLine();
    void processOneOtherLine();
    void processTwo(std::string result);
};
class FileInput : public Input {
public:
    ~FileInput();
    FileInput(std::istream* in) : Input(in) {};
    void processOneFirstLine();
    void processOneOtherLine();
    void processTwo(std::string result);
};
#endif
