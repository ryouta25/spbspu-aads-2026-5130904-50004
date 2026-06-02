#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "hashIters.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace pozdeev {

  struct StringHash {
    size_t operator()(const std::string & str) const;
  };

  struct StringEqual {
    bool operator()(const std::string & a, const std::string & b) const;
  };

  inline size_t StringHash::operator()(const std::string & str) const
  {
    size_t hash = 0x811C9DC5;
    for (size_t i = 0; i < str.length(); ++i) {
      hash ^= static_cast< size_t >(str[i]);
      hash *= 0x01000193;
    }
    return hash;
  }

  inline bool StringEqual::operator()(const std::string & a, const std::string & b) const
  {
    return a == b;
  }

  template< class Key, class Value, class Hash = StringHash, class Equal = StringEqual >
  class HashTable {
  public:
    using Iterator = HashIterator< Key, Value >;

    HashTable(size_t slots = 16);
    HashTable(const HashTable & other);
    ~HashTable();

    HashTable & operator=(HashTable other);

    void swap(HashTable & other) noexcept;

    void add(const Key & k, const Value & v);
    Value drop(const Key & k);
    bool has(const Key & k) const;
    Value & get(const Key & k);
    const Value & get(const Key & k) const;
    void rehash(size_t slots);

    Iterator begin();
    Iterator end();

  private:
    size_t capacity_;
    size_t size_;
    HashNode< Key, Value > ** table_;
    Hash hasher_;
    Equal equals_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
    capacity_(slots),
    size_(0),
    table_(new HashNode< Key, Value >*[slots])
  {
    for (size_t i = 0; i < capacity_; ++i) {
      table_[i] = nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & other):
    capacity_(other.capacity_),
    size_(other.size_),
    table_(new HashNode< Key, Value >*[other.capacity_]),
    hasher_(other.hasher_),
    equals_(other.equals_)
  {
    for (size_t i = 0; i < capacity_; ++i) {
      table_[i] = nullptr;
    }
    for (size_t i = 0; i < capacity_; ++i) {
      HashNode< Key, Value > * current = other.table_[i];
      HashNode< Key, Value > * tail = nullptr;
      while (current != nullptr) {
        HashNode< Key, Value > * newNode = new HashNode< Key, Value >(current->key_, current->value_, nullptr);
        if (table_[i] == nullptr) {
          table_[i] = newNode;
        } else {
          tail->next_ = newNode;
        }
        tail = newNode;
        current = current->next_;
      }
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    for (size_t i = 0; i < capacity_; ++i) {
      HashNode< Key, Value > * current = table_[i];
      while (current != nullptr) {
        HashNode< Key, Value > * nextNode = current->next_;
        delete current;
        current = nextNode;
      }
    }
    delete[] table_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable & other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(table_, other.table_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > & HashTable< Key, Value, Hash, Equal >::operator=(HashTable other)
  {
    swap(other);
    return *this;
  }

  ```
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "hashIters.hpp"
#include <stdexcept>
#include <utility>
#include <string>

namespace pozdeev {

  struct StringHash {
    size_t operator()(const std::string & str) const;
  };

  struct StringEqual {
    bool operator()(const std::string & a, const std::string & b) const;
  };

  inline size_t StringHash::operator()(const std::string & str) const
  {
    size_t hash = 0x811C9DC5;
    for (size_t i = 0; i < str.length(); ++i) {
      hash ^= static_cast< size_t >(str[i]);
      hash *= 0x01000193;
    }
    return hash;
  }

  inline bool StringEqual::operator()(const std::string & a, const std::string & b) const
  {
    return a == b;
  }

  template< class Key, class Value, class Hash = StringHash, class Equal = StringEqual >
  class HashTable {
  public:
    using Iterator = HashIterator< Key, Value >;

    HashTable(size_t slots = 16);
    HashTable(const HashTable & other);
    ~HashTable();

    HashTable & operator=(HashTable other);

    void swap(HashTable & other) noexcept;

    void add(const Key & k, const Value & v);
    Value drop(const Key & k);
    bool has(const Key & k) const;
    Value & get(const Key & k);
    const Value & get(const Key & k) const;
    void rehash(size_t slots);

    Iterator begin();
    Iterator end();

  private:
    size_t capacity_;
    size_t size_;
    HashNode< Key, Value > ** table_;
    Hash hasher_;
    Equal equals_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
    capacity_(slots),
    size_(0),
    table_(new HashNode< Key, Value >*[slots])
  {
    for (size_t i = 0; i < capacity_; ++i) {
      table_[i] = nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & other):
    capacity_(other.capacity_),
    size_(other.size_),
    table_(new HashNode< Key, Value >*[other.capacity_]),
    hasher_(other.hasher_),
    equals_(other.equals_)
  {
    for (size_t i = 0; i < capacity_; ++i) {
      table_[i] = nullptr;
    }
    for (size_t i = 0; i < capacity_; ++i) {
      HashNode< Key, Value > * current = other.table_[i];
      HashNode< Key, Value > * tail = nullptr;
      while (current != nullptr) {
        HashNode< Key, Value > * newNode = new HashNode< Key, Value >(current->key_, current->value_, nullptr);
        if (table_[i] == nullptr) {
          table_[i] = newNode;
        } else {
          tail->next_ = newNode;
        }
        tail = newNode;
        current = current->next_;
      }
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    for (size_t i = 0; i < capacity_; ++i) {
      HashNode< Key, Value > * current = table_[i];
      while (current != nullptr) {
        HashNode< Key, Value > * nextNode = current->next_;
        delete current;
        current = nextNode;
      }
    }
    delete[] table_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable & other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(table_, other.table_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > & HashTable< Key, Value, Hash, Equal >::operator=(HashTable other)
  {
    swap(other);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key & k, const Value & v)
  {
    if (size_ >= capacity_ * 2) {
      throw std::overflow_error("HashTable overflow: rehash required");
    }

    size_t index = hasher_(k) % capacity_;
    HashNode< Key, Value > * current = table_[index];

    while (current != nullptr) {
      if (equals_(current->key_, k)) {
        current->value_ = v;
        return;
      }
      current = current->next_;
    }

    table_[index] = new HashNode< Key, Value >(k, v, table_[index]);
    size_ = size_ + 1;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key & k)
  {
    size_t index = hasher_(k) % capacity_;
    HashNode< Key, Value > * current = table_[index];
    HashNode< Key, Value > * prev = nullptr;

    while (current != nullptr) {
      if (equals_(current->key_, k)) {
        Value val = current->value_;
        if (prev == nullptr) {
          table_[index] = current->next_;
        } else {
          prev->next_ = current->next_;
        }
        delete current;
        size_ = size_ - 1;
        return val;
      }
      prev = current;
      current = current->next_;
    }

    throw std::invalid_argument("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key & k) const
  {
    size_t index = hasher_(k) % capacity_;
    HashNode< Key, Value > * current = table_[index];

    while (current != nullptr) {
      if (equals_(current->key_, k)) {
        return true;
      }
      current = current->next_;
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::get(const Key & k)
  {
    size_t index = hasher_(k) % capacity_;
    HashNode< Key, Value > * current = table_[index];

    while (current != nullptr) {
      if (equals_(current->key_, k)) {
        return current->value_;
      }
      current = current->next_;
    }
    throw std::invalid_argument("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::get(const Key & k) const
  {
    size_t index = hasher_(k) % capacity_;
    HashNode< Key, Value > * current = table_[index];

    while (current != nullptr) {
      if (equals_(current->key_, k)) {
        return current->value_;
      }
      current = current->next_;
    }
    throw std::invalid_argument("Key not found");
  }

}

#endif
