#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <cstddef>
#include <utility>

namespace pozdeev {

template< class T >
class Queue {
public:
    Queue();
    ~Queue();

    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

    Queue(Queue &&rhs) noexcept;
    Queue &operator=(Queue &&rhs) noexcept;

    void push(T value);
    T drop();
    T front() const;
    bool isEmpty() const;
    std::size_t size() const;

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
Queue< T >::Node::Node(T data, Node *next)
    : data_(data)
    , next_(next)
{
}

template< class T >
Queue< T >::Queue()
    : head_(nullptr)
    , tail_(nullptr)
    , size_(0)
{
}

template< class T >
Queue< T >::~Queue()
{
    while (head_ != nullptr) {
        Node *temp = head_;
        head_ = head_->next_;
        delete temp;
    }
}

template< class T >
Queue< T >::Queue(Queue &&rhs) noexcept
    : head_(rhs.head_)
    , tail_(rhs.tail_)
    , size_(rhs.size_)
{
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
}

template< class T >
Queue< T > &Queue< T >::operator=(Queue &&rhs) noexcept
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
void Queue< T >::push(T value)
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
T Queue< T >::drop()
{
    if (isEmpty()) {
        throw std::runtime_error("Queue underflow");
    }
    Node *temp = head_;
    T value = temp->data_;
    head_ = head_->next_;
    if (head_ == nullptr) {
        tail_ = nullptr;
    }
    delete temp;
    --size_;
    return value;
}

template< class T >
T Queue< T >::front() const
{
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return head_->data_;
}

template< class T >
bool Queue< T >::isEmpty() const
{
    return size_ == 0;
}

template< class T >
std::size_t Queue< T >::size() const
{
    return size_;
}

}

#endif
