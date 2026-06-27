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

    int height(Node* node) const
    {
      return node ? node->height_ : 0;
    }

    int getBalance(Node* node) const
    {
      return node ? height(node->left_.get()) - height(node->right_.get()) : 0;
    }

    void updateHeight(Node* node)
    {
      if (node) {
        node->height_ = 1 + std::max(height(node->left_.get()), height(node->right_.get()));
      }
    }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y)
    {
      std::unique_ptr<Node> x = std::move(y->left_);
      y->left_ = std::move(x->right_);
      updateHeight(y.get());
      x->right_ = std::move(y);
      updateHeight(x.get());
      return x;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x)
    {
      std::unique_ptr<Node> y = std::move(x->right_);
      x->right_ = std::move(y->left_);
      updateHeight(x.get());
      y->left_ = std::move(x);
      updateHeight(y.get());
      return y;
    }
  };

}

#endif
