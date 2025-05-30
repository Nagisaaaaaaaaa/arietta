#include "Arietta/Stateful/Map.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

suite<"Map"> _ = [] {
  "Map"_test = [] {
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
};

} // namespace
