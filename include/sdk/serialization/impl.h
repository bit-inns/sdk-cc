#ifndef SERIALIZATION_IMPL_H
#define SERIALIZATION_IMPL_H

#include <array>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>

#include "concepts.h"
#include "deserializer.h"
#include "serializer.h"


namespace serialization
{


template<typename T>
struct binary_data 
{
  typedef std::conditional_t<
    std::is_const_v<std::remove_pointer_t<std::remove_reference_t<T>>>, 
    const void*, 
    void*
  > storage_type;

  storage_type data_;
  uint64_t size_;

  binary_data(T&& d, uint64_t s) : data_(std::forward<T>(d)), size_(s) {}
};

template<class T>
inline binary_data<T> make_binary_data(T&& data, size_t size) {
  return {std::forward<T>(data), size};
}

template<typename T>
void serialize(binary_serializer& serializer, const binary_data<T>& d) {
  serializer.write(d.data_, static_cast<std::streamsize>(d.size_));
}

template<typename T>
void deserialize(binary_deserializer& deserializer, binary_data<T>& d) {
  deserializer.read(d.data_, static_cast<std::streamsize>(d.size_));
}


template<typename T>
struct size_tag 
{
  typedef std::conditional_t<
    std::is_lvalue_reference_v<T>, 
    T, 
    std::decay_t<T>
  > storage_type;

  storage_type size_;

  size_tag(T&& sz) : size_(std::forward<T>(sz)) {}

  size_tag& operator=(const size_tag&) = delete;
};

template<class T>
inline size_tag<T> make_size_tag(T sz) 
{
  return {std::forward<T>(sz)};
}

template<typename T>
void serialize(binary_serializer& serializer, const size_tag<T>& tag) 
{
  serializer(tag.size_);
}

template<typename T>
void deserialize(binary_deserializer& deserializer, size_tag<T>& tag) 
{
  deserializer(tag.size_);
}


template<typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template<arithmetic T>
inline void serialize(binary_serializer& serializer, const T& v) 
{
  serializer.write(std::addressof(v), sizeof(v));
}

template<arithmetic T>
inline void deserialize(binary_deserializer& deserializer, T& v) 
{
  deserializer.read(std::addressof(v), sizeof(v));
}


template<typename T, size_t len, class Serializer>
inline void serialize(Serializer &serializer, const std::array<T, len> &arr) 
{
  serializer(make_binary_data(arr.data(), len * sizeof(T)));
}

template<class Deserializer, typename T, size_t len>
inline void deserialize(Deserializer &deserializer, std::array<T, len> &arr) 
{
  deserializer(make_binary_data(arr.data(), len * sizeof(T)));
}


template<class Serializer, class char_t, class traits, class alloc>
inline void serialize(Serializer &serializer, const std::basic_string<char_t, traits, alloc> &str) 
{
  serializer(make_size_tag(str.size()));
  serializer(make_binary_data(str.data(), str.size() * sizeof(char_t)));
}

template<class Deserializer, class char_t, class traits, class alloc>
inline void deserialize(Deserializer &deserializer, std::basic_string<char_t, traits, alloc> &str) 
{
  size_t sz;
  size_tag<size_t> tag = make_size_tag(sz);
  deserializer(tag);
  str.resize(tag.size_);
  deserializer(make_binary_data(const_cast<char_t*>(str.data()), str.size() * sizeof(char_t)) );
}


} // namespace serialization


#endif // #ifndef SERIALIZATION_IMPL_H