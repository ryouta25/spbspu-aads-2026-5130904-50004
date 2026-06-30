#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace pozdeev {

  template <typename Key, typename Value>
  class AvlTree {
  private:
    struct Node {
      Key key_;
      Value value_;
      int height_;
      Node* left_;
      Node* right_;
      Node* parent_;

      Node(const Key& key, const Value& value, Node* parent = nullptr) :
        key_(key),
        value_(value),
        height_(1),
        left_(nullptr),
        right_(nullptr),
        parent_(parent)
      {}
    };

  public:
    class Iterator {
    public:
      Iterator(Node* node) :
        current_(node)
      {}

      Iterator& operator++()
      {
        if (!current_) {
          return *this;
        }
        if (current_->right_) {
          current_ = current_->right_;
          while (current_->left_) {
            current_ = current_->left_;
          }
        } else {
          Node* parent = current_->parent_;
          while (parent && current_ == parent->right_) {
            current_ = parent;
            parent = parent->parent_;
          }
          current_ = parent;
        }
        return *this;
      }

      bool operator!=(const Iterator& other) const
      {
        return current_ != other.current_;
      }

      bool operator==(const Iterator& other) const
      {
        return current_ == other.current_;
      }

      Value& operator*()
      {
        return current_->value_;
      }

      Value* operator->()
      {
        return &(current_->value_);
      }

      const Key& key() const
      {
        return current_->key_;
      }

    private:
      Node* current_;
      friend class AvlTree;
    };

    class ConstIterator {
    public:
      ConstIterator(const Node* node) :
        current_(node)
      {}

      ConstIterator& operator++()
      {
        if (!current_) {
          return *this;
        }
        if (current_->right_) {
          current_ = current_->right_;
          while (current_->left_) {
            current_ = current_->left_;
          }
        } else {
          const Node* parent = current_->parent_;
          while (parent && current_ == parent->right_) {
            current_ = parent;
            parent = parent->parent_;
          }
          current_ = parent;
        }
        return *this;
      }

      bool operator!=(const ConstIterator& other) const
      {
        return current_ != other.current_;
      }

      bool operator==(const ConstIterator& other) const
      {
        return current_ == other.current_;
      }

      const Value& operator*() const
      {
        return current_->value_;
      }

      const Value* operator->() const
      {
        return &(current_->value_);
      }

      const Key& key() const
      {
        return current_->key_;
      }

    private:
      const Node* current_;
    };

    AvlTree() :
      root_(nullptr),
      size_(0)
    {}

    ~AvlTree()
    {
      clear();
    }

    AvlTree(const AvlTree&) = delete;
    AvlTree& operator=(const AvlTree&) = delete;

    bool insert(const Key& key, const Value& value)
    {
      if (find(key) != end()) {
        return false;
      }
      root_ = insertNode(root_, nullptr, key, value);
      ++size_;
      return true;
    }

    Iterator find(const Key& key)
    {
      Node* current = root_;
      while (current) {
        if (key == current->key_) {
          return Iterator(current);
        } else if (key < current->key_) {
          current = current->left_;
        } else {
          current = current->right_;
        }
      }
      return end();
    }

    ConstIterator find(const Key& key) const
    {
      const Node* current = root_;
      while (current) {
        if (key == current->key_) {
          return ConstIterator(current);
        } else if (key < current->key_) {
          current = current->left_;
        } else {
          current = current->right_;
        }
      }
      return end();
    }

    void clear()
    {
      destroyTree(root_);
      root_ = nullptr;
      size_ = 0;
    }

    int getHeight() const
    {
      return height(root_);
    }

    size_t getSize() const
    {
      return size_;
    }

    Iterator begin()
    {
      Node* current = root_;
      if (current) {
        while (current->left_) {
          current = current->left_;
        }
      }
      return Iterator(current);
    }

    Iterator end()
    {
      return Iterator(nullptr);
    }

    ConstIterator begin() const
    {
      const Node* current = root_;
      if (current) {
        while (current->left_) {
          current = current->left_;
        }
      }
      return ConstIterator(current);
    }

    ConstIterator end() const
    {
      return ConstIterator(nullptr);
    }

  private:
    Node* root_;
    size_t size_;

    void destroyTree(Node* node)
    {
      if (node) {
        destroyTree(node->left_);
        destroyTree(node->right_);
        delete node;
      }
    }

    int height(Node* node) const
    {
      return node ? node->height_ : 0;
    }

    int getBalance(Node* node) const
    {
      return node ? height(node->left_) - height(node->right_) : 0;
    }

    void updateHeight(Node* node)
    {
      if (node) {
        node->height_ = 1 + std::max(height(node->left_), height(node->right_));
      }
    }

    Node* rotateRight(Node* y)
    {
      Node* x = y->left_;
      y->left_ = x->right_;
      if (x->right_) {
        x->right_->parent_ = y;
      }
      x->parent_ = y->parent_;
      x->right_ = y;
      y->parent_ = x;

      updateHeight(y);
      updateHeight(x);
      return x;
    }

    Node* rotateLeft(Node* x)
    {
      Node* y = x->right_;
      x->right_ = y->left_;
      if (y->left_) {
        y->left_->parent_ = x;
      }
      y->parent_ = x->parent_;
      y->left_ = x;
      x->parent_ = y;

      updateHeight(x);
      updateHeight(y);
      return y;
    }

    Node* insertNode(Node* node, Node* parent, const Key& key, const Value& value)
    {
      if (!node) {
        return new Node(key, value, parent);
      }

      if (key < node->key_) {
        node->left_ = insertNode(node->left_, node, key, value);
      } else if (key > node->key_) {
        node->right_ = insertNode(node->right_, node, key, value);
      } else {
        return node;
      }

      updateHeight(node);
      const int balance = getBalance(node);

      if (balance > 1 && key < node->left_->key_) {
        return rotateRight(node);
      }
      if (balance < -1 && key > node->right_->key_) {
        return rotateLeft(node);
      }
      if (balance > 1 && key > node->left_->key_) {
        node->left_ = rotateLeft(node->left_);
        return rotateRight(node);
      }
      if (balance < -1 && key < node->right_->key_) {
        node->right_ = rotateRight(node->right_);
        return rotateLeft(node);
      }

      return node;
    }
  };

}

#endif
