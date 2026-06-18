#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <cstddef>
#include <utility>

namespace pozdeev {

template< class T >
class Stack {
public:
    Stack();
    ~Stack();

    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    Stack(Stack &&rhs) noexcept;
    Stack &operator=(Stack &&rhs) noexcept;

    void push(T value);
    T drop();
    T top() const;
    bool isEmpty() const;
    std::size_t size() const;

private:
    struct Node {
        T data_;
        Node *next_;

        Node(T data, Node *next);
    };

    Node *head_;
    std::size_t size_;
};

template< class T >
Stack< T >::Node::Node(T data, Node *next)
    : data_(data)
    , next_(next)
{
}

template< class T >
Stack< T >::Stack()
    : head_(nullptr)
    , size_(0)
{
}

template< class T >
Stack< T >::~Stack()
{
    while (head_ != nullptr) {
        Node *temp = head_;
        head_ = head_->next_;
        delete temp;
    }
}

template< class T >
Stack< T >::Stack(Stack &&rhs) noexcept
    : head_(rhs.head_)
    , size_(rhs.size_)
{
    rhs.head_ = nullptr;
    rhs.size_ = 0;
}

template< class T >
Stack< T > &Stack< T >::operator=(Stack &&rhs) noexcept
{
    if (this != &rhs) {
        while (head_ != nullptr) {
            Node *temp = head_;
            head_ = head_->next_;
            delete temp;
        }
        head_ = rhs.head_;
        size_ = rhs.size_;
        rhs.head_ = nullptr;
        rhs.size_ = 0;
    }
    return *this;
}

template< class T >
void Stack< T >::push(T value)
{
    head_ = new Node(value, head_);
    ++size_;
}

template< class T >
T Stack< T >::drop()
{
    if (isEmpty()) {
        throw std::runtime_error("Stack underflow");
    }
    Node *temp = head_;
    T value = temp->data_;
    head_ = head_->next_;
    delete temp;
    --size_;
    return value;
}

template< class T >
T Stack< T >::top() const
{
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty");
    }
    return head_->data_;
}

template< class T >
bool Stack< T >::isEmpty() const
{
    return size_ == 0;
}

template< class T >
std::size_t Stack< T >::size() const
{
    return size_;
}

}

#endif
