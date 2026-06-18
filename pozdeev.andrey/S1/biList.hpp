#ifndef POZDEEV_BILIST_HPP
#define POZDEEV_BILIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iterator>

namespace pozdeev
{
  template< class T >
  class BiList;

  template< class T >
  class Node
  {
    friend class BiList< T >;

  public:
    T val_;
    Node< T >* prev_;
    Node< T >* next_;

    explicit Node(const T& value);
    explicit Node(T&& value);
    Node(const T& value, Node* p, Node* n);
    Node(T&& value, Node* p, Node* n);
  };

  template< class T >
  class LIter : public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class BiList< T >;

  public:
    LIter() noexcept;
    LIter(const LIter& other) noexcept = default;
    LIter(LIter&& other) noexcept = default;
    ~LIter() = default;

    LIter& operator=(const LIter& other) noexcept = default;
    LIter& operator=(LIter&& other) noexcept = default;

    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;
    T& operator*() const;
    T* operator->() const;

    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;

  private:
    Node< T >* cur_;
    Node< T >* tail_;

    LIter(Node< T >* node, Node< T >* listTail) noexcept;
  };

  template< class T >
  class LCIter : public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class BiList< T >;

  public:
    LCIter() noexcept;
    LCIter(const LIter< T >& other) noexcept;
    LCIter(const LCIter& other) noexcept = default;
    LCIter(LCIter&& other) noexcept = default;
    ~LCIter() = default;

    LCIter& operator=(const LCIter& other) noexcept = default;
    LCIter& operator=(LCIter&& other) noexcept = default;

    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;
    const T& operator*() const;
    const T* operator->() const;

    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;

  private:
    const Node< T >* cur_;
    const Node< T >* tail_;

    LCIter(const Node< T >* node, const Node< T >* listTail) noexcept;
  };

  template< class T >
  class BiList
  {
  public:
    BiList() noexcept;
    BiList(const BiList& other);
    BiList(BiList&& other) noexcept;
    ~BiList();

    BiList& operator=(const BiList& other);
    BiList& operator=(BiList&& other) noexcept;

    void swap(BiList& other) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    LIter< T > begin() noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cend() const noexcept;

    void pushFront(const T& value);
    void pushFront(T&& value);
    void pushBack(const T& value);
    void pushBack(T&& value);
    LIter< T > insert(LIter< T > pos, const T& value);

    void popFront();
    void popBack();
    LIter< T > erase(LIter< T > pos);
    void clear() noexcept;

  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;
  };

  template< class T >
  Node< T >::Node(const T& value) :
    val_(value),
    prev_(nullptr),
    next_(nullptr)
  {
  }

  template< class T >
  Node< T >::Node(T&& value) :
    val_(std::move(value)),
    prev_(nullptr),
    next_(nullptr)
  {
  }

  template< class T >
  Node< T >::Node(const T& value, Node* p, Node* n) :
    val_(value),
    prev_(p),
    next_(n)
  {
  }

  template< class T >
  Node< T >::Node(T&& value, Node* p, Node* n) :
    val_(std::move(value)),
    prev_(p),
    next_(n)
  {
  }

  template< class T >
  LIter< T >::LIter() noexcept :
    cur_(nullptr),
    tail_(nullptr)
  {
  }

  template< class T >
  LIter< T >::LIter(Node< T >* node, Node< T >* listTail) noexcept :
    cur_(node),
    tail_(listTail)
  {
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return cur_ == other.cur_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return cur_ != other.cur_;
  }

  template< class T >
  T& LIter< T >::operator*() const
  {
    return cur_->val_;
  }

  template< class T >
  T* LIter< T >::operator->() const
  {
    return &(cur_->val_);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    if (cur_ != nullptr)
    {
      cur_ = cur_->next_;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter tmp(*this);
    if (cur_ != nullptr)
    {
      cur_ = cur_->next_;
    }
    return tmp;
  }

  template< class T >
  LIter< T >& LIter< T >::operator--() noexcept
  {
    if (cur_ == nullptr)
    {
      cur_ = tail_;
    }
    else
    {
      cur_ = cur_->prev_;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter tmp(*this);
    if (cur_ == nullptr)
    {
      cur_ = tail_;
    }
    else
    {
      cur_ = cur_->prev_;
    }
    return tmp;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept :
    cur_(nullptr),
    tail_(nullptr)
  {
  }

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& other) noexcept :
    cur_(other.cur_),
    tail_(other.tail_)
  {
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T >* node, const Node< T >* listTail) noexcept :
    cur_(node),
    tail_(listTail)
  {
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return cur_ == other.cur_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return cur_ != other.cur_;
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    return cur_->val_;
  }

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    return &(cur_->val_);
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    if (cur_ != nullptr)
    {
      cur_ = cur_->next_;
    }
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter tmp(*this);
    if (cur_ != nullptr)
    {
      cur_ = cur_->next_;
    }
    return tmp;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator--() noexcept
  {
    if (cur_ == nullptr)
    {
      cur_ = tail_;
    }
    else
    {
      cur_ = cur_->prev_;
    }
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter tmp(*this);
    if (cur_ == nullptr)
    {
      cur_ = tail_;
    }
    else
    {
      cur_ = cur_->prev_;
    }
    return tmp;
  }

  template< class T >
  BiList< T >::BiList() noexcept :
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
  }

  template< class T >
  BiList< T >::BiList(const BiList& other) :
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    LCIter< T > it = other.cbegin();
    while (it != other.cend())
    {
      pushBack(*it);
      ++it;
    }
  }

  template< class T >
  BiList< T >::BiList(BiList&& other) noexcept :
    BiList()
  {
    swap(other);
  }

  template< class T >
  BiList< T >::~BiList()
  {
    clear();
  }

  template< class T >
  void BiList< T >::swap(BiList& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< class T >
  BiList< T >& BiList< T >::operator=(const BiList& other)
  {
    if (this != &other)
    {
      BiList tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  BiList< T >& BiList< T >::operator=(BiList&& other) noexcept
  {
    swap(other);
    return *this;
  }

  template< class T >
  bool BiList< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t BiList< T >::getSize() const noexcept
  {
    return size_;
  }

  template< class T >
  T& BiList< T >::front()
  {
    if (isEmpty())
    {
      throw std::runtime_error("List is empty");
    }
    return head_->val_;
  }

  template< class T >
  const T& BiList< T >::front() const
  {
    if (isEmpty())
    {
      throw std::runtime_error("List is empty");
    }
    return head_->val_;
  }

  template< class T >
  T& BiList< T >::back()
  {
    if (isEmpty())
    {
      throw std::runtime_error("List is empty");
    }
    return tail_->val_;
  }

  template< class T >
  const T& BiList< T >::back() const
  {
    if (isEmpty())
    {
      throw std::runtime_error("List is empty");
    }
    return tail_->val_;
  }

  template< class T >
  LIter< T > BiList< T >::begin() noexcept
  {
    return LIter< T >(head_, tail_);
  }

  template< class T >
  LCIter< T > BiList< T >::cbegin() const noexcept
  {
    return LCIter< T >(head_, tail_);
  }

  template< class T >
  LIter< T > BiList< T >::end() noexcept
  {
    return LIter< T >(nullptr, tail_);
  }

  template< class T >
  LCIter< T > BiList< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr, tail_);
  }

  template< class T >
  void BiList< T >::pushFront(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    if (size_ == 0)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      newNode->next_ = head_;
      head_->prev_ = newNode;
      head_ = newNode;
    }
    size_ = size_ + 1;
  }

  template< class T >
  void BiList< T >::pushFront(T&& value)
  {
    Node< T >* newNode = new Node< T >(std::move(value));
    if (size_ == 0)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      newNode->next_ = head_;
      head_->prev_ = newNode;
      head_ = newNode;
    }
    size_ = size_ + 1;
  }

  template< class T >
  void BiList< T >::pushBack(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    if (size_ == 0)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      newNode->prev_ = tail_;
      tail_->next_ = newNode;
      tail_ = newNode;
    }
    size_ = size_ + 1;
  }

  template< class T >
  void BiList< T >::pushBack(T&& value)
  {
    Node< T >* newNode = new Node< T >(std::move(value));
    if (size_ == 0)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      newNode->prev_ = tail_;
      tail_->next_ = newNode;
      tail_ = newNode;
    }
    size_ = size_ + 1;
  }

  template< class T >
  LIter< T > BiList< T >::insert(LIter< T > pos, const T& value)
  {
    if (pos == end())
    {
      pushBack(value);
      return LIter< T >(tail_, tail_);
    }
    if (pos == begin())
    {
      pushFront(value);
      return begin();
    }
    Node< T >* newNode = new Node< T >(value, pos.cur_->prev_, pos.cur_);
    pos.cur_->prev_->next_ = newNode;
    pos.cur_->prev_ = newNode;
    size_ = size_ + 1;
    return LIter< T >(newNode, tail_);
  }

  template< class T >
  void BiList< T >::popFront()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("List is empty");
    }
    Node< T >* temp = head_;
    if (size_ == 1)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      head_ = head_->next_;
      head_->prev_ = nullptr;
    }
    delete temp;
    size_ = size_ - 1;
  }

  template< class T >
  void BiList< T >::popBack()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("List is empty");
    }
    Node< T >* temp = tail_;
    if (size_ == 1)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      tail_ = tail_->prev_;
      tail_->next_ = nullptr;
    }
    delete temp;
    size_ = size_ - 1;
  }

  template< class T >
  LIter< T > BiList< T >::erase(LIter< T > pos)
  {
    if (size_ == 0 || pos == end())
    {
      throw std::runtime_error("Can not erase");
    }
    if (pos == begin())
    {
      popFront();
      return begin();
    }
    if (pos.cur_ == tail_)
    {
      popBack();
      return end();
    }
    Node< T >* nextNode = pos.cur_->next_;
    pos.cur_->prev_->next_ = pos.cur_->next_;
    pos.cur_->next_->prev_ = pos.cur_->prev_;
    delete pos.cur_;
    size_ = size_ - 1;
    return LIter< T >(nextNode, tail_);
  }

  template< class T >
  void BiList< T >::clear() noexcept
  {
    Node< T >* current = head_;
    while (current != nullptr)
    {
      Node< T >* nextNode = current->next_;
      delete current;
      current = nextNode;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

}

#endif
