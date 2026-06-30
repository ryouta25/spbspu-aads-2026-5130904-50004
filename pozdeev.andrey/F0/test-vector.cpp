#include <boost/test/unit_test.hpp>

#include "vector.hpp"

#include <utility>

BOOST_AUTO_TEST_SUITE(VectorTests)

BOOST_AUTO_TEST_CASE(testConstructorAndPushBack)
{
  pozdeev::Vector<int> vec;
  BOOST_CHECK(vec.isEmpty());
  BOOST_CHECK_EQUAL(vec.getSize(), 0);

  vec.pushBack(10);
  vec.pushBack(20);

  BOOST_CHECK(!vec.isEmpty());
  BOOST_CHECK_EQUAL(vec.getSize(), 2);
  BOOST_CHECK_EQUAL(vec[0], 10);
  BOOST_CHECK_EQUAL(vec[1], 20);
}

BOOST_AUTO_TEST_CASE(testCopySemantics)
{
  pozdeev::Vector<int> v1;
  v1.pushBack(1);
  v1.pushBack(2);

  pozdeev::Vector<int> v2(v1);
  BOOST_CHECK_EQUAL(v2.getSize(), 2);
  BOOST_CHECK_EQUAL(v2[0], 1);

  pozdeev::Vector<int> v3;
  v3 = v1;
  BOOST_CHECK_EQUAL(v3.getSize(), 2);
  BOOST_CHECK_EQUAL(v3[1], 2);

  v1[0] = 100;
  BOOST_CHECK_EQUAL(v2[0], 1);
  BOOST_CHECK_EQUAL(v3[0], 1);
}

BOOST_AUTO_TEST_CASE(testMoveSemantics)
{
  pozdeev::Vector<int> v1;
  v1.pushBack(42);

  pozdeev::Vector<int> v2(std::move(v1));
  BOOST_CHECK_EQUAL(v2.getSize(), 1);
  BOOST_CHECK_EQUAL(v2[0], 42);
  BOOST_CHECK_EQUAL(v1.getSize(), 0);

  pozdeev::Vector<int> v3;
  v3 = std::move(v2);
  BOOST_CHECK_EQUAL(v3.getSize(), 1);
  BOOST_CHECK_EQUAL(v3[0], 42);
  BOOST_CHECK_EQUAL(v2.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  pozdeev::Vector<int> vec;
  vec.pushBack(10);
  vec.pushBack(20);
  vec.pushBack(30);

  int sum = 0;
  for (int val : vec) {
    sum += val;
  }

  BOOST_CHECK_EQUAL(sum, 60);
}

BOOST_AUTO_TEST_CASE(testClear)
{
  pozdeev::Vector<int> vec;
  vec.pushBack(1);
  vec.clear();

  BOOST_CHECK(vec.isEmpty());
  BOOST_CHECK_EQUAL(vec.getSize(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
