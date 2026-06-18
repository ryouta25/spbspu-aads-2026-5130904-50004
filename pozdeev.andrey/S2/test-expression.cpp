#include <boost/test/unit_test.hpp>

#include "expression.hpp"
#include <stdexcept>
#include <string>
#include <utility>

BOOST_AUTO_TEST_SUITE(ExpressionTests)

BOOST_AUTO_TEST_CASE(expression_simple_addition)
{
    std::string line = "1 + 2";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 3);
}

BOOST_AUTO_TEST_CASE(expression_simple_multiplication)
{
    std::string line = "4 * 5";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 20);
}

BOOST_AUTO_TEST_CASE(expression_shift_left)
{
    std::string line = "1 << 2";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 4);
}

BOOST_AUTO_TEST_CASE(expression_shift_left_five)
{
    std::string line = "5 << 1";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 10);
}

BOOST_AUTO_TEST_CASE(expression_with_parentheses)
{
    std::string line = "( 1 + 2 ) * 3";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 9);
}

BOOST_AUTO_TEST_CASE(expression_shift_with_parentheses)
{
    std::string line = "( 1 << 2 ) + 3";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 7);
}

BOOST_AUTO_TEST_CASE(expression_division_by_zero)
{
    std::string line = "10 / 0";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    BOOST_CHECK_THROW(pozdeev::evaluatePostfix(std::move(postfix)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(expression_negative_shift)
{
    std::string line = "5 << -1";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    BOOST_CHECK_THROW(pozdeev::evaluatePostfix(std::move(postfix)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(expression_mismatched_parentheses)
{
    std::string line = "( 1 + 2";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    BOOST_CHECK_THROW(pozdeev::infixToPostfix(tokens), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(expression_operator_priority)
{
    std::string line = "2 + 3 * 4";
    pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
    pozdeev::Queue< pozdeev::Token > postfix = pozdeev::infixToPostfix(tokens);
    long long result = pozdeev::evaluatePostfix(std::move(postfix));
    BOOST_CHECK_EQUAL(result, 14);
}

BOOST_AUTO_TEST_SUITE_END()
