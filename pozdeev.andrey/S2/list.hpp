#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <cstddef>
#include <utility>

namespace pozdeev {

template< class T >
class List {
public:
    List();
    ~List();

    List(const List &) = delete;
    List &operator=(const List &) = delete;

    List(List &&rhs) noexcept;
    List &operator=(List &&rhs) noexcept;

    void pushBack(T value);
    T get(std::size_t index) const;
    std::size_t size() const;
    bool isEmpty() const;

private:
    struct Node {
        T data_;
        Node *next_;

        Node(T data, Node *next);
    };

    Node *head_;
    Node *tail_;
    std::size_t size_;
};

template< class T >
List< T >::Node::Node(T data, Node *next)
    : data_(data)
    , next_(next)
{
}

template< class T >
List< T >::List()
    : head_(nullptr)
    , tail_(nullptr)
    , size_(0)
{
}

template< class T >
List< T >::~List()
{
    while (head_ != nullptr) {
        Node *temp = head_;
        head_ = head_->next_;
        delete temp;
    }
}

template< class T >
List< T >::List(List &&rhs) noexcept
    : head_(rhs.head_)
    , tail_(rhs.tail_)
    , size_(rhs.size_)
{
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
}

template< class T >
List< T > &List< T >::operator=(List &&rhs) noexcept
{
    if (this != &rhs) {
        while (head_ != nullptr) {
            Node *temp = head_;
            head_ = head_->next_;
            delete temp;
        }
        head_ = rhs.head_;
        tail_ = rhs.tail_;
        size_ = rhs.size_;
        rhs.head_ = nullptr;
        rhs.tail_ = nullptr;
        rhs.size_ = 0;
    }
    return *this;
}

template< class T >
void List< T >::pushBack(T value)
{
    Node *newNode = new Node(value, nullptr);
    if (tail_ == nullptr) {
        head_ = newNode;
        tail_ = newNode;
    } else {
        tail_->next_ = newNode;
        tail_ = newNode;
    }
    ++size_;
}

template< class T >
T List< T >::get(std::size_t index) const
{
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    Node *current = head_;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next_;
    }
    return current->data_;
}

template< class T >
std::size_t List< T >::size() const
{
    return size_;
}

template< class T >
bool List< T >::isEmpty() const
{
    return size_ == 0;
}

}

#endif
