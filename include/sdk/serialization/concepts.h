#ifndef SERIALIZATION_CONCEPTS_H
#define SERIALIZATION_CONCEPTS_H

#include <concepts>


namespace serialization
{


template<class Serializer, typename T>
concept has_member_serialize_method = requires(Serializer& serializer, const T& cls) {
  cls.serialize(serializer);
};
template<class Serializer, typename T>
concept has_non_member_serialize_method = requires(Serializer& serializer, const T& obj) {
  serialize(serializer, obj);
};


template<class Deserializer, typename T>
concept has_member_deserialize_method = requires(Deserializer& deserializer, T& cls) {
  cls.deserialize(deserializer);
};
template<class Deserializer, typename T>
concept has_non_member_deserialize_method = requires(Deserializer& deserializer, T& obj) {
  deserialize(deserializer, obj);
};


} // namespace serialization


#endif // #ifndef SERIALIZATION_CONCEPTS_H