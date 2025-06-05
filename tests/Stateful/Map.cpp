#include "Arietta/Stateful/Map.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

template <typename T>
void ImplicitInstantiationByFunction() {
  static_assert(Map<T>::template Insert<i8, u8>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(Map<T>::template Insert<i16, T>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
  SRT_EXPR(Map<T>::template Insert<T, T>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
  static_assert(std::is_same_v<typename Map<T>::template At<T>, T>);
}

template <typename T>
class ImplicitInstantiationByClass {
  static_assert(Map<T>::template Insert<i8, u8>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(Map<T>::template Insert<i16, T>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
  SRT_EXPR(Map<T>::template Insert<T, T>());
  static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
  static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
  static_assert(std::is_same_v<typename Map<T>::template At<T>, T>);
};

//
//
//
//
//
suite<"Map"> _ = [] {
  "Explicit"_test = [] {
    Map<>::Insert<i8, u8>();
    Map<int>::Insert<u8, i8>();
    Map<float>::Insert<i8, u16>();
    static_assert(std::is_same_v<Map<>::At<i8>, u8>);
    static_assert(std::is_same_v<Map<int>::At<u8>, i8>);
    static_assert(std::is_same_v<Map<float>::At<i8>, u16>);

    Map<>::Insert<i16, u16>();
    Map<int>::Insert<u16, i16>();
    Map<float>::Insert<i16, u32>();
    static_assert(std::is_same_v<Map<>::At<i8>, u8>);
    static_assert(std::is_same_v<Map<>::At<i16>, u16>);
    static_assert(std::is_same_v<Map<int>::At<u8>, i8>);
    static_assert(std::is_same_v<Map<int>::At<u16>, i16>);
    static_assert(std::is_same_v<Map<float>::At<i8>, u16>);
    static_assert(std::is_same_v<Map<float>::At<i16>, u32>);

    Map<>::Insert<i32, u32>();
    Map<int>::Insert<u32, i32>();
    Map<float>::Insert<i32, u64>();
    static_assert(std::is_same_v<Map<>::At<i8>, u8>);
    static_assert(std::is_same_v<Map<>::At<i16>, u16>);
    static_assert(std::is_same_v<Map<>::At<i32>, u32>);
    static_assert(std::is_same_v<Map<int>::At<u8>, i8>);
    static_assert(std::is_same_v<Map<int>::At<u16>, i16>);
    static_assert(std::is_same_v<Map<int>::At<u32>, i32>);
    static_assert(std::is_same_v<Map<float>::At<i8>, u16>);
    static_assert(std::is_same_v<Map<float>::At<i16>, u32>);
    static_assert(std::is_same_v<Map<float>::At<i32>, u64>);
  };

  //
  //
  //
  "Implicit"_test = [] {
    auto implicitInstantiationByLambda = []<typename T> {
      static_assert(Map<T>::template Insert<i8, u8>());
      static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
      static_assert(Map<T>::template Insert<i16, T>());
      static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
      static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
      SRT_EXPR(Map<T>::template Insert<T, T>());
      static_assert(std::is_same_v<typename Map<T>::template At<i8>, u8>);
      static_assert(std::is_same_v<typename Map<T>::template At<i16>, T>);
      static_assert(std::is_same_v<typename Map<T>::template At<T>, T>);
    };

    ImplicitInstantiationByFunction<u8>();
    ImplicitInstantiationByFunction<u8 const>();
    ImplicitInstantiationByFunction<u8 volatile>();
    ImplicitInstantiationByClass<u16>{};
    ImplicitInstantiationByClass<u16 const>{};
    ImplicitInstantiationByClass<u16 volatile>{};
    implicitInstantiationByLambda.operator()<u32>();
    implicitInstantiationByLambda.operator()<u32 const>();
    implicitInstantiationByLambda.operator()<u32 volatile>();
  };
};

} // namespace
