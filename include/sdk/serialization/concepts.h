#ifndef SDK_SERIALIZATION_CONCEPTS_H
#define SDK_SERIALIZATION_CONCEPTS_H

#include <concepts>


namespace sdk::serialization
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


} // namespace sdk::serialization


#endif // #ifndef SDK_SERIALIZATION_CONCEPTS_H