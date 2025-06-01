#pragma once

/// \file
/// \brief This file implements a compile-time type map, `Map`.
///
/// \warning This implementation relies on stateful template metaprogramming,
/// which may lead to unexpected behavior if misused or
/// relied upon across translation units.
///
/// \code
/// Map<>::Insert<i32, f32>();
/// Map<>::Insert<i64, f64>();
/// static_assert(std::is_same_v<Map<>::At<i32>, f32>);
/// static_assert(std::is_same_v<Map<>::At<i64>, f64>);
///
/// Map<u32>::Insert<i32, f64>();
/// Map<u32>::Insert<i64, f32>();
/// static_assert(std::is_same_v<Map<u32>::At<i32>, f64>);
/// static_assert(std::is_same_v<Map<u32>::At<i64>, f32>);
/// \endcode

//
//
//
//
//
#include "Arietta/Arietta.hpp"

namespace arietta::stateful {
namespace {

namespace map::detail {

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

} // namespace map::detail

//
//
//
template <typename _ = map::detail::Default, typename T = map::detail::Anon<_>>
struct Map {
  template <typename Key, typename Value>
  static consteval auto Insert() {
    map::detail::Setter<T, Key, Value> s;
    return s.value;
  }

  template <auto key, typename Value>
  static consteval auto Insert() {
    return Insert<map::detail::C<key>, Value>();
  }

  template <typename Key>
  using At = typename decltype(flag(map::detail::Reader<T, Key>{}))::type;
};

} // namespace
} // namespace arietta::stateful
