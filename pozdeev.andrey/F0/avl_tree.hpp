#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "vector.hpp"

#include <algorithm>
#include <memory>

namespace pozdeev {

  template <typename Key, typename Value>
  class AvlTree {
  public:
    AvlTree() = default;

    Value* find(const Key& key) const
    {
      Node* current = root_.get();
      while (current != nullptr) {
        if (key == current->key_) {
          return const_cast<Value*>(&(current->value_));
        } else if (key < current->key_) {
          current = current->left_.get();
        } else {
          current = current->right_.get();
        }
      }
      return nullptr;
    }

    void clear()
    {
      root_.reset();
      size_ = 0;
    }

  private:
    struct Node {
      Key key_;
      Value value_;
      int height_;
      std::unique_ptr<Node> left_;
      std::unique_ptr<Node> right_;

      Node(const Key& key, const Value& value) :
        key_(key),
        value_(value),
        height_(1)
      {}
    };

    std::unique_ptr<Node> root_;
    size_t size_ = 0;
  };

}

#endif
