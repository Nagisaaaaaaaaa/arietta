#include "Arietta/Stateful/Map.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Map"> _ = [] {
  "Map"_test = [] {
    stateful::Map<>::Insert<i8, u8>();
    stateful::Map<int>::Insert<u8, i8>();
    stateful::Map<float>::Insert<i8, u16>();
    static_assert(std::is_same_v<stateful::Map<>::Find<i8>, u8>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u8>, i8>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i8>, u16>);

    stateful::Map<>::Insert<i16, u16>();
    stateful::Map<int>::Insert<u16, i16>();
    stateful::Map<float>::Insert<i16, u32>();
    static_assert(std::is_same_v<stateful::Map<>::Find<i8>, u8>);
    static_assert(std::is_same_v<stateful::Map<>::Find<i16>, u16>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u8>, i8>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u16>, i16>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i8>, u16>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i16>, u32>);

    stateful::Map<>::Insert<i32, u32>();
    stateful::Map<int>::Insert<u32, i32>();
    stateful::Map<float>::Insert<i32, u64>();
    static_assert(std::is_same_v<stateful::Map<>::Find<i8>, u8>);
    static_assert(std::is_same_v<stateful::Map<>::Find<i16>, u16>);
    static_assert(std::is_same_v<stateful::Map<>::Find<i32>, u32>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u8>, i8>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u16>, i16>);
    static_assert(std::is_same_v<stateful::Map<int>::Find<u32>, i32>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i8>, u16>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i16>, u32>);
    static_assert(std::is_same_v<stateful::Map<float>::Find<i32>, u64>);
  };
};

} // namespace
