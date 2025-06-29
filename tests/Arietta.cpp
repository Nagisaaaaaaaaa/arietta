#include "Arietta/Arietta.hpp"

#include <atomic>
#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

class A {};

enum class B : int { e };

void foo();

//
//
//
//
//
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

  //
  //
  //
  "Is And Is Not"_test = [] {
    static_assert(is::Arithmetic<bool>);
    static_assert(is::Arithmetic<char>);
    static_assert(is::Arithmetic<char const>);
    static_assert(is::Arithmetic<int>);
    static_assert(is::Arithmetic<int const>);
    static_assert(is::Arithmetic<float>);
    static_assert(is::Arithmetic<float const>);
    static_assert(is::Arithmetic<std::size_t>);

    static_assert(!is::Arithmetic<char &>);
    static_assert(!is::Arithmetic<char *>);
    static_assert(!is::Arithmetic<int &>);
    static_assert(!is::Arithmetic<int *>);
    static_assert(!is::Arithmetic<float &>);
    static_assert(!is::Arithmetic<float *>);
    static_assert(!is::Arithmetic<A>);
    static_assert(!is::Arithmetic<B>);
    static_assert(!is::Arithmetic<decltype(B::e)>);
    static_assert(!is::Arithmetic<std::byte>);
    static_assert(!is::Arithmetic<std::atomic_int>);
    static_assert(!is::Arithmetic<std::false_type>);
    static_assert(!is::Arithmetic<std::integral_constant<int, 0>>);

    static_assert(!isnot::Arithmetic<bool>);
    static_assert(!isnot::Arithmetic<char>);
    static_assert(!isnot::Arithmetic<char const>);
    static_assert(!isnot::Arithmetic<int>);
    static_assert(!isnot::Arithmetic<int const>);
    static_assert(!isnot::Arithmetic<float>);
    static_assert(!isnot::Arithmetic<float const>);
    static_assert(!isnot::Arithmetic<std::size_t>);

    static_assert(isnot::Arithmetic<char &>);
    static_assert(isnot::Arithmetic<char *>);
    static_assert(isnot::Arithmetic<int &>);
    static_assert(isnot::Arithmetic<int *>);
    static_assert(isnot::Arithmetic<float &>);
    static_assert(isnot::Arithmetic<float *>);
    static_assert(isnot::Arithmetic<A>);
    static_assert(isnot::Arithmetic<B>);
    static_assert(isnot::Arithmetic<decltype(B::e)>);
    static_assert(isnot::Arithmetic<std::byte>);
    static_assert(isnot::Arithmetic<std::atomic_int>);
    static_assert(isnot::Arithmetic<std::false_type>);
    static_assert(isnot::Arithmetic<std::integral_constant<int, 0>>);

    static_assert(is::Same<void, void>);
    static_assert(is::Same<A, A>);

    static_assert(!is::Same<A, void>);
    static_assert(!is::Same<A, A const>);
    static_assert(!is::Same<A, A volatile>);
    static_assert(!is::Same<A, A *>);
    static_assert(!is::Same<A, A &>);
    static_assert(!is::Same<A, A &&>);

    static_assert(!isnot::Same<void, void>);
    static_assert(!isnot::Same<A, A>);

    static_assert(isnot::Same<A, void>);
    static_assert(isnot::Same<A, A const>);
    static_assert(isnot::Same<A, A volatile>);
    static_assert(isnot::Same<A, A *>);
    static_assert(isnot::Same<A, A &>);
    static_assert(isnot::Same<A, A &&>);

    static_assert(is::Void<void>);
    static_assert(is::Void<void const>);
    static_assert(is::Void<void volatile>);
    static_assert(is::Void<void const volatile>);
    static_assert(is::Void<std::void_t<>>);

    static_assert(!is::Void<void *>);
    static_assert(!is::Void<int>);
    static_assert(!is::Void<decltype(foo)>);
    static_assert(!is::Void<std::is_void<void>>);

    static_assert(!isnot::Void<void>);
    static_assert(!isnot::Void<void const>);
    static_assert(!isnot::Void<void volatile>);
    static_assert(!isnot::Void<void const volatile>);
    static_assert(!isnot::Void<std::void_t<>>);

    static_assert(isnot::Void<void *>);
    static_assert(isnot::Void<int>);
    static_assert(isnot::Void<decltype(foo)>);
    static_assert(isnot::Void<std::is_void<void>>);
  };
};

} // namespace
