#pragma once
#ifndef IO_H
#define IO_H
#include<string>
#include"eval_env.h"
#include<iostream>
#include<stack>
#include"token.h"
//使用模板设计模式完成io
class Inputer {
protected:
    std::unique_ptr<std::istream> in;
public:
    Inputer(std::istream* in) : in(in) {};
    void processInput(std::shared_ptr<EvalEnv> env);
    virtual void processOneFirstLine() = 0;
    virtual void processOneOtherLine() = 0;
    virtual void processTwo(std::string result) = 0;
    static std::unique_ptr<Inputer> parseArgs(int argc, char** argv);
    std::string strip_comment(const std::string& line);
    bool checkParen(const std::deque<TokenPtr>& tokens,
                    std::stack<char> parenContainer);
};

class ReplInputer : public Inputer {
public:
    ReplInputer(std::istream* in) : Inputer(in) {};
    void processOneFirstLine();
    void processOneOtherLine();
    void processTwo(std::string result);
};
class FileInputer : public Inputer {
public:
    FileInputer(std::istream* in) : Inputer(in) {};
    void processOneFirstLine();
    void processOneOtherLine();
    void processTwo(std::string result);
};
#endif
