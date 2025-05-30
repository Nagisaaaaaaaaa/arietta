#include "Arietta/Stateful/Map.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Map"> _ = [] {
  "Map"_test = [] {
    stateful::Insert<i8, u8>();
    static_assert(std::is_same_v<stateful::Find<i8>, u8>);

    stateful::Insert<i16, u16>();
    static_assert(std::is_same_v<stateful::Find<i8>, u8>);
    static_assert(std::is_same_v<stateful::Find<i16>, u16>);

    stateful::Insert<i32, u32>();
    static_assert(std::is_same_v<stateful::Find<i8>, u8>);
    static_assert(std::is_same_v<stateful::Find<i16>, u16>);
    static_assert(std::is_same_v<stateful::Find<i32>, u32>);
  };
};

} // namespace
