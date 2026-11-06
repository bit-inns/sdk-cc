#ifndef SDK_SERIALIZATION_SERIALIZER_H
#define SDK_SERIALIZATION_SERIALIZER_H

#include <iostream>

#include "concepts.h"


namespace sdk::serialization
{


template<class Impl>
class serializer_base 
{
private:

  Impl* self_;

public:

  serializer_base(Impl* const derived) : self_(derived) {}

  template<typename... Ts> 
  inline Impl& operator() (Ts&&... objs) {
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
  requires has_member_serialize_method<Impl, T>
  void process_impl(const T& obj) {
    obj.serialize(*self_);
  }
  template<typename T>
  requires has_non_member_serialize_method<Impl, T>
  void process_impl(const T& obj) {
    serialize(*self_, obj);
  }
};


class binary_serializer : public serializer_base<binary_serializer> 
{
private:
  
  std::ostream& stream_;

public: 

  binary_serializer(std::ostream &s)
    : serializer_base<binary_serializer>(this), stream_(s) {}

  ~binary_serializer() noexcept = default;

  void write(const void* data, std::streamsize size) {
    auto const written_size = stream_.rdbuf()->sputn(
      reinterpret_cast<const char*>(data), size
    );
    if(written_size != size) {
      throw std::runtime_error(
        "serialization : binary_serializer.write: incomplete write"
      );
    };
  }
};


} // namespace sdk::serialization


#endif // #ifndef SDK_SERIALIZATION_SERIALIZER_H