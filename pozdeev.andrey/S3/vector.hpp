#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>

namespace pozdeev {

  template< class T >
  class Vector {
  public:
    Vector();
    Vector(const Vector< T > & other);
    ~Vector();

    Vector< T > & operator=(Vector< T > other);

    void swap(Vector< T > & other) noexcept;

    void pushBack(const T & value);
    void erase(size_t index);
    size_t size() const;
    void clear();

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

    T * begin();
    T * end();
    const T * begin() const;
    const T * end() const;

  private:
    size_t size_;
    size_t capacity_;
    T * data_;
  };

  template< class T >
  Vector< T >::Vector():
    size_(0),
    capacity_(4),
    data_(new T[4])
  {}

  template< class T >
  Vector< T >::Vector(const Vector< T > & other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(new T[other.capacity_])
  {
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  template< class T >
  Vector< T >::~Vector()
  {
    delete[] data_;
  }

  template< class T >
  void Vector< T >::swap(Vector< T > & other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  template< class T >
  Vector< T > & Vector< T >::operator=(Vector< T > other)
  {
    swap(other);
    return *this;
  }

  template< class T >
  void Vector< T >::pushBack(const T & value)
  {
    if (size_ == capacity_) {
      capacity_ *= 2;
      T * newData = new T[capacity_];
      for (size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
      }
      delete[] data_;
      data_ = newData;
    }
    data_[size_] = value;
    size_ = size_ + 1;
  }

  template< class T >
  void Vector< T >::erase(size_t index)
  {
    if (index < size_) {
      for (size_t i = index; i < size_ - 1; ++i) {
        data_[i] = data_[i + 1];
      }
      size_ = size_ - 1;
    }
  }

  template< class T >
  size_t Vector< T >::size() const
  {
    return size_;
  }

  template< class T >
  void Vector< T >::clear()
  {
    size_ = 0;
  }

  template< class T >
  T & Vector< T >::operator[](size_t index)
  {
    return data_[index];
  }

  template< class T >
  const T & Vector< T >::operator[](size_t index) const
  {
    return data_[index];
  }

  template< class T >
  T * Vector< T >::begin()
  {
    return data_;
  }

  template< class T >
  T * Vector< T >::end()
  {
    return data_ + size_;
  }

  template< class T >
  const T * Vector< T >::begin() const
  {
    return data_;
  }

  template< class T >
  const T * Vector< T >::end() const
  {
    return data_ + size_;
  }

}

#endif
