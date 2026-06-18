#include "expression.hpp"

#include <cstdlib>
#include <limits>
#include <string>

pozdeev::Token::Token()
    : type_(TokenType::Number)
    , value_()
    , numberValue_(0)
{
}

int pozdeev::getPriority(const std::string &op)
{
    if (op == "*" || op == "/" || op == "%") {
        return 3;
    }
    if (op == "+" || op == "-") {
        return 2;
    }
    if (op == "<<") {
        return 1;
    }
    return 0;
}

bool pozdeev::isOperator(const std::string &token)
{
    return token == "+" || token == "-" || token == "*" || token == "/"
        || token == "%" || token == "<<";
}

long long pozdeev::applyOperator(long long left, long long right, const std::string &op)
{
    if (op == "+") {
        if ((right > 0 && left > std::numeric_limits< long long >::max() - right) ||
            (right < 0 && left < std::numeric_limits< long long >::min() - right)) {
            throw std::runtime_error("Integer overflow");
        }
        return left + right;
    }
    if (op == "-") {
        if ((right < 0 && left > std::numeric_limits< long long >::max() + right) ||
            (right > 0 && left < std::numeric_limits< long long >::min() + right)) {
            throw std::runtime_error("Integer overflow");
        }
        return left - right;
    }
    if (op == "*") {
        if (left != 0 && right != 0) {
            if (left > 0 && right > 0
                && left > std::numeric_limits< long long >::max() / right) {
                throw std::runtime_error("Integer overflow");
            }
            if (left < 0 && right < 0
                && left < std::numeric_limits< long long >::max() / right) {
                throw std::runtime_error("Integer overflow");
            }
            if (left > 0 && right < 0
                && right < std::numeric_limits< long long >::min() / left) {
                throw std::runtime_error("Integer overflow");
            }
            if (left < 0 && right > 0
                && left < std::numeric_limits< long long >::min() / right) {
                throw std::runtime_error("Integer overflow");
            }
        }
        return left * right;
    }
    if (op == "/") {
        if (right == 0) {
            throw std::runtime_error("Division by zero");
        }
        if (left == std::numeric_limits< long long >::min() && right == -1) {
            throw std::runtime_error("Integer overflow");
        }
        return left / right;
    }
    if (op == "%") {
        if (right == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        long long result = left % right;
        if (result < 0 && right > 0) {
            result += right;
        }
        return result;
    }
    if (op == "<<") {
        if (right < 0) {
            throw std::runtime_error("Negative shift count");
        }
        if (right >= 64 || left < 0) {
            throw std::runtime_error("Invalid shift operation");
        }
        if (left > 0
            && left > (std::numeric_limits< long long >::max() >> right)) {
            throw std::runtime_error("Integer overflow");
        }
        return left << right;
    }
    throw std::runtime_error("Unknown operator");
}

pozdeev::Token pozdeev::parseToken(const std::string &str)
{
    Token token;
    token.value_ = str;

    if (str == "(") {
        token.type_ = TokenType::LeftParen;
    } else if (str == ")") {
        token.type_ = TokenType::RightParen;
    } else if (isOperator(str)) {
        token.type_ = TokenType::Operator;
    } else {
        token.type_ = TokenType::Number;
        try {
            token.numberValue_ = std::stoll(str);
        } catch (...) {
            throw std::runtime_error("Invalid number: " + str);
        }
    }
    return token;
}

pozdeev::Queue< pozdeev::Token > pozdeev::infixToPostfix(
    const List< Token > &infix)
{
    Queue< Token > postfix;
    Stack< Token > opStack;

    for (std::size_t i = 0; i < infix.size(); ++i) {
        const Token &token = infix.get(i);
        if (token.type_ == TokenType::Number) {
            postfix.push(token);
        } else if (token.type_ == TokenType::LeftParen) {
            opStack.push(token);
        } else if (token.type_ == TokenType::RightParen) {
            while (!opStack.isEmpty()
                && opStack.top().type_ != TokenType::LeftParen) {
                postfix.push(opStack.drop());
            }
            if (opStack.isEmpty()) {
                throw std::runtime_error("Mismatched parentheses");
            }
            opStack.drop();
        } else if (token.type_ == TokenType::Operator) {
            while (!opStack.isEmpty()
                && opStack.top().type_ == TokenType::Operator) {
                if (getPriority(opStack.top().value_)
                    >= getPriority(token.value_)) {
                    postfix.push(opStack.drop());
                } else {
                    break;
                }
            }
            opStack.push(token);
        }
    }

    while (!opStack.isEmpty()) {
        if (opStack.top().type_ == TokenType::LeftParen) {
            throw std::runtime_error("Mismatched parentheses");
        }
        postfix.push(opStack.drop());
    }

    return postfix;
}

long long pozdeev::evaluatePostfix(Queue< Token > postfix)
{
    Stack< long long > evalStack;

    while (!postfix.isEmpty()) {
        const Token &token = postfix.front();
        postfix.drop();
        if (token.type_ == TokenType::Number) {
            evalStack.push(token.numberValue_);
        } else if (token.type_ == TokenType::Operator) {
            if (evalStack.size() < 2) {
                throw std::runtime_error("Invalid expression structure");
            }
            long long right = evalStack.drop();
            long long left = evalStack.drop();
            long long result = applyOperator(left, right, token.value_);
            evalStack.push(result);
        }
    }

    if (evalStack.size() != 1) {
        throw std::runtime_error("Invalid expression structure");
    }

    return evalStack.drop();
}

pozdeev::List< pozdeev::Token > pozdeev::tokenizeLine(const std::string &line)
{
    List< Token > tokens;
    std::string currentToken;

    for (std::size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == ' ') {
            if (!currentToken.empty()) {
                tokens.pushBack(parseToken(currentToken));
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    if (!currentToken.empty()) {
        tokens.pushBack(parseToken(currentToken));
    }

    return tokens;
}
