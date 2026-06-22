#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(bstree_push_and_get)
{
  pozdeev::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
  BOOST_CHECK(tree.has(5));
  BOOST_CHECK(!tree.has(10));
}

BOOST_AUTO_TEST_CASE(bstree_get_invalid_key)
{
  pozdeev::BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(bstree_drop_and_size)
{
  pozdeev::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");

  BOOST_CHECK_EQUAL(tree.drop(10), "ten");
  BOOST_CHECK(!tree.has(10));
  BOOST_CHECK(tree.has(5));
  BOOST_CHECK(!tree.empty());
}

BOOST_AUTO_TEST_CASE(bstree_iterator_inorder)
{
  pozdeev::BSTree< int, std::string > tree;
  tree.push(20, "b");
  tree.push(10, "a");
  tree.push(30, "c");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->first, 10);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 20);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 30);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(bstree_rotations)
{
  pozdeev::BSTree< int, std::string > tree;
  tree.push(10, "root");
  tree.push(5, "left");
  tree.push(15, "right");

  auto itRoot = tree.begin();
  ++itRoot;

  auto newRootIt = tree.rotateLeft(itRoot);

  BOOST_CHECK_EQUAL(newRootIt->first, 15);

  BOOST_CHECK_EQUAL(itRoot->first, 10);
}

BOOST_AUTO_TEST_SUITE_END()
