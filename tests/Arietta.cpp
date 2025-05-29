#include "Arietta/Arietta.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Arietta"> _ = [] {
  "Aliases"_test = [] {
    static_assert(sizeof(i8) == 1);
    static_assert(sizeof(u8) == 1);
    static_assert(sizeof(i16) == 2);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(i32) == 4);
    static_assert(sizeof(u32) == 4);
    static_assert(sizeof(i64) == 8);
    static_assert(sizeof(u64) == 8);
    static_assert(sizeof(isize) == sizeof(void *));
    static_assert(sizeof(usize) == sizeof(void *));

    static_assert(std::is_signed_v<i8>);
    static_assert(!std::is_signed_v<u8>);
    static_assert(std::is_signed_v<i16>);
    static_assert(!std::is_signed_v<u16>);
    static_assert(std::is_signed_v<i32>);
    static_assert(!std::is_signed_v<u32>);
    static_assert(std::is_signed_v<i64>);
    static_assert(!std::is_signed_v<u64>);
    static_assert(std::is_signed_v<isize>);
    static_assert(!std::is_signed_v<usize>);

    static_assert(sizeof(f32) == 4);
    static_assert(sizeof(f64) == 8);

    static_assert(std::is_signed_v<f32>);
    static_assert(std::is_signed_v<f64>);
  };
};

} // namespace
