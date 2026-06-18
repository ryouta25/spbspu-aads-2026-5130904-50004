#ifndef HASHITERS_HPP
#define HASHITERS_HPP

#include <cstddef>
#include <iterator>

namespace pozdeev {

  template< class Key, class Value >
  struct HashNode {
    Key key_;
    Value value_;
    HashNode * next_;

    HashNode(const Key & k, const Value & v, HashNode * n);
  };

  template< class Key, class Value >
  HashNode< Key, Value >::HashNode(const Key & k, const Value & v, HashNode * n):
    key_(k),
    value_(v),
    next_(n)
  {}

  template< class Key, class Value >
  class HashIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = Value*;
    using reference = Value&;

    HashIterator();
    HashIterator(HashNode< Key, Value > ** table, size_t capacity, size_t index, HashNode< Key, Value > * node);

    HashIterator< Key, Value > & operator++();
    HashIterator< Key, Value > operator++(int);

    bool operator==(const HashIterator< Key, Value > & other) const;
    bool operator!=(const HashIterator< Key, Value > & other) const;

    reference operator*();
    pointer operator->();

    const Key & getKey() const;

  private:
    HashNode< Key, Value > ** table_;
    size_t capacity_;
    size_t index_;
    HashNode< Key, Value > * current_;
  };

  template< class Key, class Value >
  HashIterator< Key, Value >::HashIterator():
    table_(nullptr),
    capacity_(0),
    index_(0),
    current_(nullptr)
  {}

  template< class Key, class Value >
  HashIterator< Key, Value >::HashIterator(HashNode< Key, Value > ** table, size_t capacity, size_t index, HashNode< Key, Value > * node):
    table_(table),
    capacity_(capacity),
    index_(index),
    current_(node)
  {
    if (current_ == nullptr && table_ != nullptr) {
      while (index_ < capacity_ && table_[index_] == nullptr) {
        index_ = index_ + 1;
      }
      if (index_ < capacity_) {
        current_ = table_[index_];
      }
    }
  }

  template< class Key, class Value >
  HashIterator< Key, Value > & HashIterator< Key, Value >::operator++()
  {
    if (current_ != nullptr) {
      current_ = current_->next_;
    }
    while (current_ == nullptr) {
      index_ = index_ + 1;
      if (index_ >= capacity_) {
        break;
      }
      current_ = table_[index_];
    }
    return *this;
  }

  template< class Key, class Value >
  HashIterator< Key, Value > HashIterator< Key, Value >::operator++(int)
  {
    HashIterator< Key, Value > temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  bool HashIterator< Key, Value >::operator==(const HashIterator< Key, Value > & other) const
  {
    return current_ == other.current_;
  }

  template< class Key, class Value >
  bool HashIterator< Key, Value >::operator!=(const HashIterator< Key, Value > & other) const
  {
    return current_ != other.current_;
  }

  template< class Key, class Value >
  Value & HashIterator< Key, Value >::operator*()
  {
    return current_->value_;
  }

  template< class Key, class Value >
  Value * HashIterator< Key, Value >::operator->()
  {
    return &(current_->value_);
  }

  template< class Key, class Value >
  const Key & HashIterator< Key, Value >::getKey() const
  {
    return current_->key_;
  }

}

#endif
