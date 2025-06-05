#pragma once

/// \file
/// \brief This file implements a compile-time type vector, `Vector`.
///
/// \warning This implementation relies on stateful template metaprogramming,
/// which may lead to unexpected behavior if misused or
/// relied upon across translation units.
///
/// \code
/// SRT_EXPR(Vector<>::PushBack<i32>());
/// SRT_EXPR(Vector<>::PushBack<i64>());
/// static_assert(Vector<>::Size() == 2);
/// static_assert(std::is_same_v<Vector<>::At<0>, i32>);
/// static_assert(std::is_same_v<Vector<>::At<1>, i64>);
/// static_assert(std::is_same_v<Vector<>::Back<>, i64>);
///
/// SRT_EXPR(Vector<T>::PushBack<f32>());
/// SRT_EXPR(Vector<T>::PushBack<f64>());
/// static_assert(Vector<T>::Size() == 2);
/// static_assert(std::is_same_v<Vector<T>::At<0>, f32>);
/// static_assert(std::is_same_v<Vector<T>::At<1>, f64>);
/// static_assert(std::is_same_v<Vector<T>::Back<>, f64>);
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

namespace detail::vector {

struct Default {};

template <typename>
struct Anon {};

template <auto>
struct C {};

} // namespace detail::vector

//
//
//
template <typename _ = detail::vector::Default, typename T = detail::vector::Anon<_>>
struct Vector {
  template <auto tag = []() {}>
  [[nodiscard]] static consteval auto Size() {
    return Index<T>::Load();
  }

  template <typename Value, auto tag = []() {}>
  static consteval auto PushBack() {
    constexpr usize i = Index<T>::FetchAdd();
    return Map<T>::template Insert<detail::vector::C<i>, Value>();
  }

  template <usize i>
  using At = typename Map<T>::template At<detail::vector::C<i>>;

  template <auto tag = []() {}>
    requires(Size<tag>() > 0)
  using Back = At<Size<tag>() - 1>;
};

} // namespace
} // namespace arietta::stateful
