#pragma once

/// \file
/// \brief This file implements a compile-time type vector, `Vector`.
///
/// \warning This implementation relies on stateful template metaprogramming,
/// which may lead to unexpected behavior if misused or
/// relied upon across translation units.
///
/// \code
/// static_assert(Vector<>::PushBack<i32>());
/// static_assert(Vector<>::PushBack<i64>());
/// static_assert(std::is_same_v<Vector<>::At<0>, i32>);
/// static_assert(std::is_same_v<Vector<>::At<1>, i64>);
/// static_assert(Vector<>::Size() == 2);
///
/// static_assert(Vector<T>::PushBack<f32>());
/// static_assert(Vector<T>::PushBack<f64>());
/// static_assert(std::is_same_v<Vector<T>::At<0>, f32>);
/// static_assert(std::is_same_v<Vector<T>::At<1>, f64>);
/// static_assert(Vector<T>::Size() == 2);
/// \endcode

//
//
//
//
//
#include "Arietta/Stateful/Index.hpp"
#include "Arietta/Stateful/Map.hpp"

namespace arietta::stateful {
namespace {

namespace vector::detail {

struct Default {};

template <typename>
struct Anon {};

template <auto>
struct C {};

} // namespace vector::detail

//
//
//
template <typename _ = vector::detail::Default, typename T = vector::detail::Anon<_>>
struct Vector {
  template <typename Value, auto tag = []() {}>
  static consteval auto PushBack() {
    constexpr usize i = Index<T>::FetchAdd();
    return Map<T>::template Insert<vector::detail::C<i>, Value>();
  }

  template <usize i>
  using At = typename Map<T>::template At<vector::detail::C<i>>;

  template <auto tag = []() {}>
  [[nodiscard]] static consteval auto Size() {
    return Index<T>::Load();
  }
};

} // namespace
} // namespace arietta::stateful
