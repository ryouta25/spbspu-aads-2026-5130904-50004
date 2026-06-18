#include <boost/test/unit_test.hpp>

#include "stack.hpp"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(stack_push_and_drop)
{
    pozdeev::Stack< int > stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    BOOST_CHECK_EQUAL(stack.top(), 3);
    BOOST_CHECK_EQUAL(stack.drop(), 3);
    BOOST_CHECK_EQUAL(stack.drop(), 2);
    BOOST_CHECK_EQUAL(stack.drop(), 1);
    BOOST_CHECK(stack.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_drop_empty)
{
    pozdeev::Stack< int > stack;
    BOOST_CHECK_THROW(stack.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_top_empty)
{
    pozdeev::Stack< int > stack;
    BOOST_CHECK_THROW(stack.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_size)
{
    pozdeev::Stack< int > stack;
    BOOST_CHECK_EQUAL(stack.size(), 0);
    stack.push(1);
    BOOST_CHECK_EQUAL(stack.size(), 1);
    stack.push(2);
    BOOST_CHECK_EQUAL(stack.size(), 2);
    stack.drop();
    BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
