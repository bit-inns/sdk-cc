#ifndef SERIALIZATION_DESERIALIZER_H
#define SERIALIZATION_DESERIALIZER_H

#include <iostream>

#include "concepts.h"


namespace serialization
{


template<class Impl>
class deserializer_base 
{
private:

  Impl* self_;

public:

  deserializer_base(Impl* const derived) : self_(derived) {}

  template<typename... Ts> 
  inline Impl& operator() (Ts&& ... objs) {
    self_->process(std::forward<Ts>(objs)...);
    return *self_;
  }

public:

  template<typename F, typename... Ts>
  inline void process(F&& first, Ts&&... others) {
    self_->process(std::forward<F>(first));
    self_->process(std::forward<Ts>(others)...);
  }

  template<typename T>
  inline void process(T&& obj) {
    self_->process_impl(obj);
  }

  template<typename T> 
  requires has_member_deserialize_method<Impl, T>
  void process_impl(T& obj) {
    obj.deserialize(*self_);
  }

  template<typename T>
  requires has_non_member_deserialize_method<Impl, T>
  void process_impl(T& obj) {
    deserialize(*self_, obj);
  }
};


class binary_deserializer : public deserializer_base<binary_deserializer> 
{
private:

  std::istream& stream_;

public:

  binary_deserializer(std::istream &s)
    : deserializer_base<binary_deserializer>(this), stream_(s) {}

  ~binary_deserializer() noexcept = default;

  void read(void* data, std::streamsize size) {
    auto const read_size = stream_.rdbuf()->sgetn(
      reinterpret_cast<char*>(data), size
    );
    if(read_size != size) { 
      throw std::runtime_error("serialization::binary_deserializer.read: incomplete read");
    }
  }
};


} // namespace serialization


#endif // #ifndef SERIALIZATION_DESERIALIZER_H