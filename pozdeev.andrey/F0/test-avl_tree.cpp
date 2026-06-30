#include <boost/test/unit_test.hpp>
#include <string>

#include "avl_tree.hpp"

BOOST_AUTO_TEST_SUITE(AvlTree_tests)

using Tree = pozdeev::AvlTree<std::string, int>;

BOOST_AUTO_TEST_CASE(test_insert_and_find)
{
  Tree tree;

  BOOST_CHECK(tree.getSize() == 0);

  BOOST_CHECK(tree.insert("scenario_a", 10) == true);
  BOOST_CHECK(tree.insert("scenario_b", 20) == true);
  BOOST_CHECK(tree.insert("scenario_c", 30) == true);

  BOOST_CHECK_EQUAL(tree.getSize(), 3);

  auto itA = tree.find("scenario_a");
  BOOST_CHECK(itA != tree.end());
  BOOST_CHECK_EQUAL(*itA, 10);

  auto itC = tree.find("scenario_c");
  BOOST_CHECK(itC != tree.end());
  BOOST_CHECK_EQUAL(*itC, 30);

  auto itNotFound = tree.find("unknown");
  BOOST_CHECK(itNotFound == tree.end());

  BOOST_CHECK(tree.insert("scenario_a", 99) == false);
  BOOST_CHECK_EQUAL(tree.getSize(), 3);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  Tree tree;

  tree.insert("a", 1);
  tree.insert("b", 2);

  BOOST_CHECK_EQUAL(tree.getSize(), 2);

  tree.clear();

  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK(tree.find("a") == tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
