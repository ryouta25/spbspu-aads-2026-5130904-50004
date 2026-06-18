#include <boost/test/unit_test.hpp>

#include "list.hpp"
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(list_push_back_and_get)
{
    pozdeev::List< int > list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    BOOST_CHECK_EQUAL(list.get(0), 1);
    BOOST_CHECK_EQUAL(list.get(1), 2);
    BOOST_CHECK_EQUAL(list.get(2), 3);
}

BOOST_AUTO_TEST_CASE(list_get_invalid_index)
{
    pozdeev::List< int > list;
    list.pushBack(1);
    BOOST_CHECK_THROW(list.get(1), std::out_of_range);
    BOOST_CHECK_THROW(list.get(100), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(list_get_empty)
{
    pozdeev::List< int > list;
    BOOST_CHECK_THROW(list.get(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(list_size)
{
    pozdeev::List< int > list;
    BOOST_CHECK_EQUAL(list.size(), 0);
    list.pushBack(1);
    BOOST_CHECK_EQUAL(list.size(), 1);
    list.pushBack(2);
    BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
