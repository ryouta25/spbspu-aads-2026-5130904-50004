#define BOOST_TEST_MODULE BiListTests
#include <boost/test/included/unit_test.hpp>
#include "biList.hpp"

using namespace pozdeev;

BOOST_AUTO_TEST_CASE(isEmptyCheck)
{
  BiList<int> lst;
  BOOST_CHECK(lst.isEmpty());
  lst.pushBack(1);
  BOOST_CHECK(!lst.isEmpty());
}

BOOST_AUTO_TEST_CASE(getSizeCheck)
{
  BiList<int> lst;
  BOOST_CHECK_EQUAL(lst.getSize(), 0);
  lst.pushBack(1);
  lst.pushBack(2);
  BOOST_CHECK_EQUAL(lst.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(frontCheck)
{
  BiList<int> lst;
  lst.pushBack(10);
  lst.pushBack(20);
  BOOST_CHECK_EQUAL(lst.front(), 10);
  lst.front() = 100;
  BOOST_CHECK_EQUAL(lst.front(), 100);
}

BOOST_AUTO_TEST_CASE(backCheck)
{
  BiList<int> lst;
  lst.pushBack(10);
  lst.pushBack(20);
  BOOST_CHECK_EQUAL(lst.back(), 20);
  lst.back() = 200;
  BOOST_CHECK_EQUAL(lst.back(), 200);
}

BOOST_AUTO_TEST_CASE(pushFrontCheck)
{
  BiList<int> lst;
  lst.pushFront(5);
  lst.pushFront(15);
  BOOST_CHECK_EQUAL(lst.front(), 15);
  BOOST_CHECK_EQUAL(lst.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(pushBackCheck)
{
  BiList<int> lst;
  lst.pushBack(10);
  lst.pushBack(20);
  BOOST_CHECK_EQUAL(lst.back(), 20);
  BOOST_CHECK_EQUAL(lst.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(insertCheck)
{
  BiList<int> lst;
  lst.pushBack(1);
  lst.pushBack(3);
  auto it = lst.begin();
  lst.insert(it, 0);
  BOOST_CHECK_EQUAL(lst.front(), 0);
  auto it2 = lst.begin();
  ++it2; ++it2;
  lst.insert(it2, 2);

  BOOST_CHECK_EQUAL(lst.getSize(), 4);
}

BOOST_AUTO_TEST_CASE(popFrontCheck)
{
  BiList<int> lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.popFront();
  BOOST_CHECK_EQUAL(lst.front(), 2);
  BOOST_CHECK_EQUAL(lst.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(popBackCheck)
{
  BiList<int> lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.popBack();
  BOOST_CHECK_EQUAL(lst.back(), 1);
  BOOST_CHECK_EQUAL(lst.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(eraseCheck)
{
  BiList<int> lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  auto it = lst.begin();
  ++it;
  lst.erase(it);
  BOOST_CHECK_EQUAL(lst.getSize(), 2);
  BOOST_CHECK_EQUAL(lst.front(), 1);
  BOOST_CHECK_EQUAL(lst.back(), 3);
}

BOOST_AUTO_TEST_CASE(clearCheck)
{
  BiList<int> lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  lst.clear();
  BOOST_CHECK(lst.isEmpty());
  BOOST_CHECK_EQUAL(lst.getSize(), 0);
}
