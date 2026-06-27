#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>
#include <new>

namespace pozdeev {

  template <typename T>
  class Vector {
  public:
    Vector();
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    void pushBack(const T& value);
    void pushBack(T&& value);

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;
    void clear() noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void swap(Vector& other) noexcept;

    T* begin() noexcept;
    const T* begin() const noexcept;
    T* end() noexcept;
    const T* end() const noexcept;

  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reserve(size_t newCap);
  };

  template <typename T>
  Vector<T>::Vector() :
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
  }

  template <typename T>
  Vector<T>::Vector(const Vector& other) :
    data_(other.capacity_ ? static_cast<T*>(::operator new(other.capacity_ * sizeof(T))) : nullptr),
    size_(0),
    capacity_(other.capacity_)
  {
    try {
      for (; size_ < other.size_; ++size_) {
        new (data_ + size_) T(other.data_[size_]);
      }
    } catch (...) {
      clear();
      ::operator delete(data_);
      throw;
    }
  }

  template <typename T>
  Vector<T>::Vector(Vector&& other) noexcept :
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template <typename T>
  Vector<T>::~Vector()
  {
    clear();
    ::operator delete(data_);
  }

  template <typename T>
  Vector<T>& Vector<T>::operator=(const Vector& other)
  {
    if (this != &other) {
      Vector tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template <typename T>
  Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
  {
    if (this != &other) {
      clear();
      ::operator delete(data_);
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;

      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  template <typename T>
  void Vector<T>::pushBack(const T& value)
  {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
  }

  template <typename T>
  void Vector<T>::pushBack(T&& value)
  {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  template <typename T>
  size_t Vector<T>::getSize() const noexcept
  {
    return size_;
  }

  template <typename T>
  bool Vector<T>::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template <typename T>
  void Vector<T>::clear() noexcept
  {
    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    size_ = 0;
  }

  template <typename T>
  T& Vector<T>::operator[](size_t index)
  {
    return data_[index];
  }

  template <typename T>
  const T& Vector<T>::operator[](size_t index) const
  {
    return data_[index];
  }

  template <typename T>
  void Vector<T>::swap(Vector& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template <typename T>
  T* Vector<T>::begin() noexcept
  {
    return data_;
  }

  template <typename T>
  const T* Vector<T>::begin() const noexcept
  {
    return data_;
  }

  template <typename T>
  T* Vector<T>::end() noexcept
  {
    return data_ + size_;
  }

  template <typename T>
  const T* Vector<T>::end() const noexcept
  {
    return data_ + size_;
  }

  template <typename T>
  void Vector<T>::reserve(size_t newCap)
  {
    if (newCap <= capacity_) {
      return;
    }
    T* newData = static_cast<T*>(::operator new(newCap * sizeof(T)));
    size_t constructed = 0;
    try {
      for (; constructed < size_; ++constructed) {
        new (newData + constructed) T(std::move(data_[constructed]));
      }
    } catch (...) {
      for (size_t j = 0; j < constructed; ++j) {
        newData[j].~T();
      }
      ::operator delete(newData);
      throw;
    }

    clear();
    ::operator delete(data_);
    data_ = newData;
    size_ = constructed;
    capacity_ = newCap;
  }

}

#endif
