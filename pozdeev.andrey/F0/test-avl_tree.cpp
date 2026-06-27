#include <boost/test/unit_test.hpp>

#include "avl_tree.hpp"

#include <string>

BOOST_AUTO_TEST_SUITE(AvlTreeTests)

BOOST_AUTO_TEST_CASE(testInsertAndFind)
{
  pozdeev::AvlTree<std::string, int> tree;

  BOOST_CHECK(tree.insert("scenario_a", 100));
  BOOST_CHECK(tree.insert("scenario_b", 200));

  BOOST_CHECK(!tree.insert("scenario_a", 300));

  int* valA = tree.find("scenario_a");
  BOOST_REQUIRE(valA != nullptr);
  BOOST_CHECK_EQUAL(*valA, 100);

  int* valC = tree.find("scenario_c");
  BOOST_CHECK(valC == nullptr);
}

BOOST_AUTO_TEST_CASE(testClearAndSize)
{
  pozdeev::AvlTree<int, std::string> tree;

  BOOST_CHECK_EQUAL(tree.getSize(), 0);

  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(3, "three");

  BOOST_CHECK_EQUAL(tree.getSize(), 3);

  tree.clear();
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK(tree.find(1) == nullptr);
}

BOOST_AUTO_TEST_CASE(testBalancingAndHeight)
{
  pozdeev::AvlTree<int, int> tree;

  tree.insert(1, 10);
  tree.insert(2, 20);
  tree.insert(3, 30);

  BOOST_CHECK_EQUAL(tree.getHeight(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
