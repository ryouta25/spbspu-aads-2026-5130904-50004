#include <boost/test/unit_test.hpp>

#include "queue.hpp"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_and_drop)
{
    pozdeev::Queue< int > queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    BOOST_CHECK_EQUAL(queue.front(), 1);
    BOOST_CHECK_EQUAL(queue.drop(), 1);
    BOOST_CHECK_EQUAL(queue.front(), 2);
    BOOST_CHECK_EQUAL(queue.drop(), 2);
    BOOST_CHECK_EQUAL(queue.front(), 3);
    BOOST_CHECK_EQUAL(queue.drop(), 3);
    BOOST_CHECK(queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_drop_empty)
{
    pozdeev::Queue< int > queue;
    BOOST_CHECK_THROW(queue.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_front_empty)
{
    pozdeev::Queue< int > queue;
    BOOST_CHECK_THROW(queue.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_size)
{
    pozdeev::Queue< int > queue;
    BOOST_CHECK_EQUAL(queue.size(), 0);
    queue.push(1);
    BOOST_CHECK_EQUAL(queue.size(), 1);
    queue.push(2);
    BOOST_CHECK_EQUAL(queue.size(), 2);
    queue.drop();
    BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
