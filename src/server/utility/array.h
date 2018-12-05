#ifndef UTILITY_ARRAY_H
#define UTILITY_ARRAY_H

#include <stdexcept>
#include <cstring>
#include <cmath>
#include "nstr.h"

namespace srlib {
  typedef uint8_t Byte;
  
  template<typename T>
  class Array;
  
  template<typename T>
  class Slice;
  
  /// Array<T> is a safety, const array.
  /// With RVO/NRVO, it can return without any cost.
  /// It is a RAII class, so you need not to consider its lifetime.
  /// When you try to access a index out of range, it will throw a out_of_range exception.
  /// It override operator[], so you can access it like raw array.
  /// It declare iterator, so it can be easy to use in STL algorithms.
  template<typename T>
  class Array {
  public:
    typedef T value_type;
    typedef size_t size_type;
    typedef value_type *pointer;
    typedef const pointer const_pointer;
    typedef value_type *iterator;
    typedef const iterator const_iterator;
    typedef value_type &reference;
    typedef const reference const_reference;
  protected:
    T *_data;
    size_t _size;
    template<typename U>
    static void swap(U &&lhs, U &&rhs) {
      U &&temp = std::forward<U &&>(lhs);
      lhs = std::forward<U &&>(rhs);
      rhs = std::forward<U &&>(temp);
    }
  public:
    Array() : _data(nullptr), _size(0) {}
    Array(size_t size) : _size(size) {
      try {
        _data = new T[size];
      } catch (std::bad_alloc &err) {
        _data = nullptr;
      }
    }
    Array(std::initializer_list<T> l) : _size(l.size()) {
      try {
        _data = new T[_size];
      } catch (std::bad_alloc &err) {
        _data = nullptr;
      }
      memmove(_data, l.begin(), sizeof(T) * _size);
    }
    Array(const Array &rhs) {
      _size = rhs._size;
      _data = new T[rhs._size];
      memmove(_data, rhs._data, _size * sizeof(T));
    }
    Array(Array &&rhs) noexcept {
      _size = rhs._size;
      _data = rhs._data;
      rhs._data = nullptr;
    }
    Array &operator=(const Array &rhs) {
      if (_data != nullptr) {
        delete[]_data;
        _data = nullptr;
      }
      _size = rhs._size;
      _data = new T[rhs._size];
      memmove(_data, rhs._data, _size * sizeof(T));
      return *this;
    }
    Array &operator=(Array &&rhs) noexcept {
      _size = rhs._size;
      _data = rhs._data;
      rhs._data = nullptr;
      return *this;
    }
    ~Array() {delete[]_data;}
    reference operator[](size_type i) {return _data[i];}
    const_reference operator[](size_type i) const {return _data[i];}
    reference At(size_type i) {
      if (i >= _size)throw std::out_of_range("Index out of range");
      return _data[i];
    }
    const_reference At(size_type i) const {
      if (i >= _size)throw std::out_of_range("Index out of range");
      return _data[i];
    }
    value_type Average() const {
      T sum{};
      for (int i = 0; i < _size; ++i) {
        sum += _data[i];
      }
      return sum / _size;
    }
    value_type Variance() const {
      T avg = Average();
      T sum{};
      for (int i = 0; i < _size; ++i) {
        sum += std::pow(_data[i] - avg, 2);
      }
      return sum / _size;
    }
    value_type StandardDeviation() const {
      return std::sqrt(Variance());
    }
    value_type SampleStandardDeviation() const {
      T avg = Average();
      T sum{};
      for (int i = 0; i < _size; ++i) {
        sum += std::pow(_data[i] - avg, 2);
      }
      return std::sqrt(sum / (_size - 1));
    }
    value_type AClassuncertainty() const {
      T avg = Average();
      T sum{};
      for (int i = 0; i < _size; ++i) {
        sum += std::pow(_data[i] - avg, 2);
      }
      return std::sqrt(sum / (_size * (_size - 1)));
    }
    bool Empty() const {return _size == 0;}
    void Fill(const_reference t) {for (auto &elem:*this) {elem = t;}}
    void Swap(Array &rhs) {swap(_size, rhs._size), swap(_data, rhs._data);}
    size_type Size() const {return _size;}
    pointer Data() {return _data;}
    const_pointer Data() const {return _data;}
    iterator begin() {return _data;}
    const_iterator begin() const {return _data;}
    iterator end() {return _data + _size;}
    const_iterator end() const {return _data + _size;}
    Slice<T> operator()(size_type from, size_type to = std::numeric_limits<size_type>::max()) {
      if (from >= _size)from = _size;
      if (to >= _size)to = _size;
      return Slice<T>(_data + from, to - from);
    }
    Array SubArray(size_type from, size_type to = std::numeric_limits<size_type>::max()) {
      return Array(_data + from, to - from);
    }
    size_t Find(const char *sub) {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      auto pos = memmem(Data(), Size(), sub, strlen(sub));
      if (pos == nullptr)return size_t(-1);
      return (char *) pos - Data();
    }
    size_type Find(const void *sub, size_type size) const {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      auto pos = memmem(Data(), Size(), sub, size);
      if (pos == nullptr)return size_t(-1);
      return (pointer) pos - Data();
    }
    String ToString() {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      return String(_data, _size);
    }
    String ToStringTrunc() {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      return String(_data);
    }
  };
  
  /// Slice is a slice from a Array.
  /// Slice can be created by Array(from,to).
  /// Slice will share the memory with the Array which create it.
  /// Slice will not release the memory that it points to.
  /// But when corresponding Array destroyed, Slice will be invalid.
  /// Use ToArray() can create a new Array from Slice.
  template<typename T>
  class Slice {
  public:
    typedef T value_type;
    typedef size_t size_type;
    typedef value_type *pointer;
    typedef const pointer const_pointer;
    typedef value_type *iterator;
    typedef const iterator const_iterator;
    typedef value_type &reference;
    typedef const reference const_reference;
  private:
    pointer _ptr;
    size_type _size;
  public:
    Slice() = delete;
    Slice(pointer ptr, size_type size) : _ptr(ptr), _size(size) {}
    Array<T> ToArray() {return Array<T>(_ptr, _size);}
    Slice operator()(size_type from, size_type to = std::numeric_limits<size_type>::max()) {
      if (from >= _size)from = _size;
      if (to >= _size)to = _size;
      return Slice(_ptr + from, to - from);
    }
    reference operator[](size_t size) {return _ptr[size];}
    const_reference operator[](size_type size) const {return _ptr[size];}
    iterator begin() {return _ptr;}
    const_iterator begin() const {return _ptr;}
    iterator end() {return _ptr + _size;}
    const_iterator end() const {return _ptr + _size;}
    size_type Size() const {return _size;}
    pointer Data() {return _ptr;}
    const_pointer Data() const {return _ptr;}
    bool Empty() const {return _size == 0;}
    String ToString() {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      return String(_ptr, _size);
    }
    String ToStringTrunc() {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      return String(_ptr);
    }
    size_type Find(const char *sub) const {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      auto pos = memmem(Data(), Size(), sub, strlen(sub));
      if (pos == nullptr)return size_t(-1);
      return (pointer) pos - Data();
    }
    size_type Find(const void *sub, size_type size) const {
      static_assert(std::is_same<T, char>::value || std::is_same<T, uint8_t>::value);
      auto pos = memmem(Data(), Size(), sub, size);
      if (pos == nullptr)return size_t(-1);
      return (pointer) pos - Data();
    }
  };
};

#endif
