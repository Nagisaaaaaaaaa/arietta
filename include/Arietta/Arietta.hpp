#pragma once

/// \file
/// \warning "Higher beings, these words are for you alone.
/// Beyond this point you enter the land of Arietta.
/// Step across this threshold and obey our laws."

//
//
//
//
//
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace arietta {

// Aliases for fundamental types.

using i8 = std::int8_t;
using u8 = std::uint8_t;

using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;

using isize = std::ptrdiff_t;
using usize = std::size_t;

using f32 = float;
using f64 = double;

//
//
//
namespace is {

namespace detail::arietta {

template <typename>
struct IsArithmetic : std::false_type {};

template <typename T>
  requires std::is_arithmetic_v<T>
struct IsArithmetic<T> : std::true_type {};

} // namespace detail::arietta

template <typename T>
concept Arithmetic = detail::arietta::IsArithmetic<T>::value;

template <typename F, typename... Args>
concept Invocable = std::invocable<F, Args...>;

template <class T, class U>
concept Same = std::same_as<T, U>;

template <typename T>
concept Void = std::is_void_v<T>;

} // namespace is

namespace isnot {

template <typename T>
concept Arithmetic = !is::Arithmetic<T>;

template <typename F, typename... Args>
concept Invocable = !is::Invocable<F, Args...>;

template <class T, class U>
concept Same = !is::Same<T, U>;

template <typename T>
concept Void = !is::Void<T>;

} // namespace isnot

//
//
//
template <is::Arithmetic T, typename F>
ART_SPECIFIER constexpr void ForEach(T const &n, F &&f) {
  for (std::decay_t<T> i{}; i < n; ++i) {
    static_assert(is::Void<decltype(f(i))>, "Function must have void return type");
    f(i);
  }
}

} // namespace arietta
