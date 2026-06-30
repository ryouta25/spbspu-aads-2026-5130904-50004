#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <memory>
#include <utility>

namespace pozdeev {

  template <typename T>
  class Vector {
  public:
    Vector();
    ~Vector();
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    void pushBack(const T& value);
    void pushBack(T&& value);
    void popBack();
    size_t getSize() const;
    bool isEmpty() const;
    void clear();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t newCapacity);
  };

  template <typename T>
  Vector<T>::Vector() :
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template <typename T>
  Vector<T>::~Vector()
  {
    clear();
    ::operator delete(data_);
  }

  template <typename T>
  Vector<T>::Vector(const Vector& other) :
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    if (capacity_ > 0) {
      data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
      for (size_t i = 0; i < size_; ++i) {
        new (data_ + i) T(other.data_[i]);
      }
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
  Vector<T>& Vector<T>::operator=(const Vector& other)
  {
    if (this != std::addressof(other)) {
      Vector<T> temp(other);
      std::swap(data_, temp.data_);
      std::swap(size_, temp.size_);
      std::swap(capacity_, temp.capacity_);
    }
    return *this;
  }

  template <typename T>
  Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
  {
    if (this != std::addressof(other)) {
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
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
  }

  template <typename T>
  void Vector<T>::pushBack(T&& value)
  {
    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  template <typename T>
  void Vector<T>::popBack()
  {
    if (size_ > 0) {
      --size_;
      data_[size_].~T();
    }
  }

  template <typename T>
  size_t Vector<T>::getSize() const
  {
    return size_;
  }

  template <typename T>
  bool Vector<T>::isEmpty() const
  {
    return size_ == 0;
  }

  template <typename T>
  void Vector<T>::clear()
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
  T* Vector<T>::begin()
  {
    return data_;
  }

  template <typename T>
  T* Vector<T>::end()
  {
    return data_ + size_;
  }

  template <typename T>
  const T* Vector<T>::begin() const
  {
    return data_;
  }

  template <typename T>
  const T* Vector<T>::end() const
  {
    return data_ + size_;
  }

  template <typename T>
  void Vector<T>::reallocate(size_t newCapacity)
  {
    T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
    for (size_t i = 0; i < size_; ++i) {
      new (newData + i) T(std::move(data_[i]));
      data_[i].~T();
    }
    ::operator delete(data_);
    data_ = newData;
    capacity_ = newCapacity;
  }

  template <typename Iterator, typename Comparator>
  void quickSort(Iterator first, Iterator last, Comparator comp)
  {
    if (last - first > 1) {
      Iterator left = first;
      Iterator right = last - 1;
      auto pivot = *(first + (last - first) / 2);
      while (left <= right) {
        while (comp(*left, pivot)) {
          ++left;
        }
        while (comp(pivot, *right)) {
          --right;
        }
        if (left <= right) {
          std::swap(*left, *right);
          ++left;
          --right;
        }
      }
      quickSort(first, right + 1, comp);
      quickSort(left, last, comp);
    }
  }

}

#endif
