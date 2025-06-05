#pragma once

/// \file
/// \brief This file implements a compile-time type map, `Map`.
///
/// \warning This implementation relies on stateful template metaprogramming,
/// which may lead to unexpected behavior if misused or
/// relied upon across translation units.
///
/// \code
/// static_assert(Map<>::Insert<i32, f32>());
/// static_assert(Map<>::Insert<i64, f64>());
/// static_assert(std::is_same_v<Map<>::At<i32>, f32>);
/// static_assert(std::is_same_v<Map<>::At<i64>, f64>);
///
/// static_assert(Map<T>::Insert<i32, f64>());
/// static_assert(Map<T>::Insert<i64, f32>());
/// static_assert(std::is_same_v<Map<T>::At<i32>, f64>);
/// static_assert(std::is_same_v<Map<T>::At<i64>, f32>);
/// \endcode

//
//
//
//
//
#include "Arietta/Stateful/Stateful.hpp"

namespace arietta::stateful {
namespace {

namespace detail::map {

template <typename T, typename Key>
struct Reader {
  friend consteval auto flag(Reader<T, Key>);
};

template <typename Value>
struct Wrapper {
  using type = Value;
};

struct Invalid {};

template <typename Value>
struct Return {
  [[nodiscard]] consteval operator bool() const { return !std::is_same_v<Value, Invalid>; }
};

template <typename T, typename Key, typename Value>
struct Setter {
  friend consteval auto flag(Reader<T, Key>) { return Wrapper<Value>{}; }

  static constexpr Return<Value> value{};
};

struct Default {};

template <typename>
struct Anon {};

template <auto>
struct C {};

} // namespace detail::map

//
//
//
template <typename _ = detail::map::Default, typename T = detail::map::Anon<_>>
struct Map {
  template <typename Key, typename Value>
  static consteval auto Insert() {
    detail::map::Setter<T, Key, Value> s;
    return s.value;
  }

  template <auto key, typename Value>
  static consteval auto Insert() {
    return Insert<detail::map::C<key>, Value>();
  }

  template <typename Key>
  using At = typename decltype(flag(detail::map::Reader<T, Key>{}))::type;
};

} // namespace
} // namespace arietta::stateful
