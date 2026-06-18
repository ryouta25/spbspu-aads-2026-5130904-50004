#include <boost/test/unit_test.hpp>
#include "hashTable.hpp"
#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(hashtable_add_and_has)
{
  pozdeev::HashTable< std::string, int > table;
  table.add("apple", 100);
  table.add("banana", 200);

  BOOST_CHECK(table.has("apple"));
  BOOST_CHECK(table.has("banana"));
  BOOST_CHECK(!table.has("orange"));
}

BOOST_AUTO_TEST_CASE(hashtable_copy_and_swap)
{
  pozdeev::HashTable< std::string, int > table1;
  table1.add("apple", 100);

  pozdeev::HashTable< std::string, int > table2;
  table2 = table1;

  BOOST_CHECK(table2.has("apple"));
  BOOST_CHECK_EQUAL(table2.get("apple"), 100);
}

BOOST_AUTO_TEST_CASE(hashtable_iterator)
{
  pozdeev::HashTable< std::string, int > table(4);
  table.add("first", 1);

  auto it = table.begin();
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_SUITE_END()
