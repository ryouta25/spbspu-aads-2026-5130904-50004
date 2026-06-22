#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <utility>
#include <stdexcept>
#include <iterator>
#include <algorithm>

namespace pozdeev {

  struct NodeBase {
    NodeBase * parent_;
    NodeBase * left_;
    NodeBase * right_;

    NodeBase() : parent_(nullptr), left_(nullptr), right_(nullptr) {}
  };

  template< class Key, class Value >
  struct Node : public NodeBase {
    std::pair< const Key, Value > data_;

    Node(const Key & k, const Value & v) :
      NodeBase(),
      data_(k, v)
    {
    }
  };

  template< class Key, class Value >
  class BSTConstIterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    BSTConstIterator();
    BSTConstIterator(const NodeBase * node);

    BSTConstIterator & operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator & operator--();
    BSTConstIterator operator--(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const BSTConstIterator & other) const;
    bool operator!=(const BSTConstIterator & other) const;

    const NodeBase * node_;
  };

  template< class Key, class Value >
  class BSTIterator : public BSTConstIterator< Key, Value > {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    BSTIterator();
    BSTIterator(NodeBase * node);

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    reference operator*() const;
    pointer operator->() const;
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree & other);
    ~BSTree();

    BSTree & operator=(BSTree other);
    void swap(BSTree & other) noexcept;

    void push(const Key & k, const Value & v);
    Value & get(const Key & k);
    const Value & get(const Key & k) const;
    Value drop(const Key & k);
    bool has(const Key & k) const;
    bool empty() const;
    void clear();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    NodeBase fakeRoot_;
    size_t size_;
    Compare comp_;

    Node< Key, Value > * getRoot() const;
    void setRoot(NodeBase * newRoot);
    NodeBase * findNode(const Key & k) const;
    void copyTree(const NodeBase * node, NodeBase * parent);
    void deleteTree(NodeBase * node);
    void replaceNode(NodeBase * u, NodeBase * v);
    size_t calculateHeight(const NodeBase * node) const;
  };

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator() :
    node_(nullptr)
  {
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const NodeBase * node) :
    node_(node)
  {
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > & BSTConstIterator< Key, Value >::operator++()
  {
    if (node_->right_) {
      node_ = node_->right_;
      while (node_->left_) {
        node_ = node_->left_;
      }
    } else {
      const NodeBase * p = node_->parent_;
      while (p && node_ == p->right_) {
        node_ = p;
        p = p->parent_;
      }
      node_ = p;
    }
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator++(int)
  {
    BSTConstIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > & BSTConstIterator< Key, Value >::operator--()
  {
    if (node_->left_) {
      node_ = node_->left_;
      while (node_->right_) {
        node_ = node_->right_;
      }
    } else {
      const NodeBase * p = node_->parent_;
      while (p && node_ == p->left_) {
        node_ = p;
        p = p->parent_;
      }
      node_ = p;
    }
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator--(int)
  {
    BSTConstIterator temp = *this;
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  typename BSTConstIterator< Key, Value >::reference BSTConstIterator< Key, Value >::operator*() const
  {
    return static_cast< const Node< Key, Value > * >(node_)->data_;
  }

  template< class Key, class Value >
  typename BSTConstIterator< Key, Value >::pointer BSTConstIterator< Key, Value >::operator->() const
  {
    return &(static_cast< const Node< Key, Value > * >(node_)->data_);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator & other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator() :
    BSTConstIterator< Key, Value >()
  {
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(NodeBase * node) :
    BSTConstIterator< Key, Value >(node)
  {
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator++()
  {
    BSTConstIterator< Key, Value >::operator++();
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator temp = *this;
    BSTConstIterator< Key, Value >::operator++();
    return temp;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator--()
  {
    BSTConstIterator< Key, Value >::operator--();
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator temp = *this;
    BSTConstIterator< Key, Value >::operator--();
    return temp;
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::reference BSTIterator< Key, Value >::operator*() const
  {
    return const_cast< reference >(BSTConstIterator< Key, Value >::operator*());
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::pointer BSTIterator< Key, Value >::operator->() const
  {
    return const_cast< pointer >(BSTConstIterator< Key, Value >::operator->());
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree() :
    size_(0)
  {
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other) :
    size_(0),
    comp_(other.comp_)
  {
    if (other.getRoot()) {
      copyTree(other.getRoot(), &fakeRoot_);
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
  {
    NodeBase * thisRoot = getRoot();
    NodeBase * otherRoot = other.getRoot();

    if (thisRoot) {
      thisRoot->parent_ = &(other.fakeRoot_);
    }
    if (otherRoot) {
      otherRoot->parent_ = &fakeRoot_;
    }

    std::swap(fakeRoot_.left_, other.fakeRoot_.left_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(BSTree other)
  {
    swap(other);
    return *this;
  }

  template< class Key, class Value, class Compare >
  Node< Key, Value > * BSTree< Key, Value, Compare >::getRoot() const
  {
    return static_cast< Node< Key, Value > * >(fakeRoot_.left_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::setRoot(NodeBase * newRoot)
  {
    fakeRoot_.left_ = newRoot;
    if (newRoot) {
      newRoot->parent_ = &fakeRoot_;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::copyTree(const NodeBase * node, NodeBase * parent)
  {
    const Node< Key, Value > * typedNode = static_cast< const Node< Key, Value > * >(node);
    Node< Key, Value > * newNode = new Node< Key, Value >(typedNode->data_.first, typedNode->data_.second);

    newNode->parent_ = parent;
    if (parent == &fakeRoot_) {
      parent->left_ = newNode;
    } else if (comp_(typedNode->data_.first, static_cast< Node< Key, Value > * >(parent)->data_.first)) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }

    size_ = size_ + 1;

    if (node->left_) {
      copyTree(node->left_, newNode);
    }
    if (node->right_) {
      copyTree(node->right_, newNode);
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::deleteTree(NodeBase * node)
  {
    if (node) {
      deleteTree(node->left_);
      deleteTree(node->right_);
      delete static_cast< Node< Key, Value > * >(node);
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    deleteTree(getRoot());
    fakeRoot_.left_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key & k, const Value & v)
  {
    NodeBase * current = getRoot();
    NodeBase * parent = &fakeRoot_;
    bool isLeft = true;

    while (current) {
      parent = current;
      const Key & currentKey = static_cast< Node< Key, Value > * >(current)->data_.first;

      if (comp_(k, currentKey)) {
        current = current->left_;
        isLeft = true;
      } else if (comp_(currentKey, k)) {
        current = current->right_;
        isLeft = false;
      } else {
        static_cast< Node< Key, Value > * >(current)->data_.second = v;
        return;
      }
    }

    Node< Key, Value > * newNode = new Node< Key, Value >(k, v);
    newNode->parent_ = parent;

    if (parent == &fakeRoot_) {
      parent->left_ = newNode;
    } else if (isLeft) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }

    size_ = size_ + 1;
  }

  template< class Key, class Value, class Compare >
  NodeBase * BSTree< Key, Value, Compare >::findNode(const Key & k) const
  {
    NodeBase * current = getRoot();
    while (current) {
      const Key & currentKey = static_cast< Node< Key, Value > * >(current)->data_.first;
      if (comp_(k, currentKey)) {
        current = current->left_;
      } else if (comp_(currentKey, k)) {
        current = current->right_;
      } else {
        return current;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::has(const Key & k) const
  {
    return findNode(k) != nullptr;
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::get(const Key & k)
  {
    NodeBase * node = findNode(k);
    if (!node) {
      throw std::out_of_range("Key not found in BSTree");
    }
    return static_cast< Node< Key, Value > * >(node)->data_.second;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::get(const Key & k) const
  {
    NodeBase * node = findNode(k);
    if (!node) {
      throw std::out_of_range("Key not found in BSTree");
    }
    return static_cast< const Node< Key, Value > * >(node)->data_.second;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::replaceNode(NodeBase * u, NodeBase * v)
  {
    if (u->parent_->left_ == u) {
      u->parent_->left_ = v;
    } else {
      u->parent_->right_ = v;
    }
    if (v) {
      v->parent_ = u->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(const Key & k)
  {
    NodeBase * z = findNode(k);
    if (!z) {
      throw std::out_of_range("Key not found in BSTree");
    }

    Value droppedValue = static_cast< Node< Key, Value > * >(z)->data_.second;

    if (!z->left_) {
      replaceNode(z, z->right_);
    } else if (!z->right_) {
      replaceNode(z, z->left_);
    } else {
      NodeBase * y = z->right_;
      while (y->left_) {
        y = y->left_;
      }
      if (y->parent_ != z) {
        replaceNode(y, y->right_);
        y->right_ = z->right_;
        y->right_->parent_ = y;
      }
      replaceNode(z, y);
      y->left_ = z->left_;
      y->left_->parent_ = y;
    }

    delete static_cast< Node< Key, Value > * >(z);
    size_ = size_ - 1;

    return droppedValue;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::begin()
  {
    NodeBase * curr = fakeRoot_.left_;
    if (curr) {
      while (curr->left_) {
        curr = curr->left_;
      }
    } else {
      curr = &fakeRoot_;
    }
    return iterator(curr);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::end()
  {
    return iterator(&fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::begin() const
  {
    const NodeBase * curr = fakeRoot_.left_;
    if (curr) {
      while (curr->left_) {
        curr = curr->left_;
      }
    } else {
      curr = &fakeRoot_;
    }
    return const_iterator(curr);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(&fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::cbegin() const
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::cend() const
  {
    return end();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    NodeBase * x = const_cast< NodeBase * >(it.node_);
    NodeBase * y = x->right_;

    if (!y) {
      return it;
    }

    x->right_ = y->left_;
    if (y->left_) {
      y->left_->parent_ = x;
    }

    replaceNode(x, y);
    y->left_ = x;
    x->parent_ = y;

    return const_iterator(y);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    NodeBase * x = const_cast< NodeBase * >(it.node_);
    NodeBase * y = x->left_;

    if (!y) {
      return it;
    }

    x->left_ = y->right_;
    if (y->right_) {
      y->right_->parent_ = x;
    }

    replaceNode(x, y);
    y->right_ = x;
    x->parent_ = y;

    return const_iterator(y);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    rotateRight(const_iterator(it.node_->right_));
    return rotateLeft(it);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    rotateLeft(const_iterator(it.node_->left_));
    return rotateRight(it);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::calculateHeight(const NodeBase * node) const
  {
    if (!node) {
      return 0;
    }
    size_t leftHeight = calculateHeight(node->left_);
    size_t rightHeight = calculateHeight(node->right_);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return calculateHeight(it.node_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return calculateHeight(getRoot());
  }

}

#endif
