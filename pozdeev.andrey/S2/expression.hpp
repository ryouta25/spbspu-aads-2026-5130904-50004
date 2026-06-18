#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "stack.hpp"
#include "queue.hpp"
#include "list.hpp"

#include <string>
#include <stdexcept>

namespace pozdeev {

enum class TokenType {
    Number,
    Operator,
    LeftParen,
    RightParen
};

struct Token {
    TokenType type_;
    std::string value_;
    long long numberValue_;

    Token();
};

int getPriority(const std::string &op);

bool isOperator(const std::string &token);

long long applyOperator(long long left, long long right, const std::string &op);

Token parseToken(const std::string &str);

Queue< Token > infixToPostfix(const List< Token > &infix);

long long evaluatePostfix(Queue< Token > postfix);

List< Token > tokenizeLine(const std::string &line);

}

#endif
