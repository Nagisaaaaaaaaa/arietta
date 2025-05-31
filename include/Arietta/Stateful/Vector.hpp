#pragma once

#include "Arietta/Stateful/Index.hpp"
#include "Arietta/Stateful/Map.hpp"

namespace arietta::stateful {

namespace vector::detail {

struct Default {};

template <auto v>
struct C {};

} // namespace vector::detail

template <typename T = vector::detail::Default>
struct Vector {
  template <typename Value, auto tag = []() {}>
  static consteval auto PushBack() {
    constexpr usize i = FetchAdd<T, tag>();
    return Map<T>::template Insert<vector::detail::C<i>, Value>();
  }

  template <usize i>
  using At = typename Map<T>::template At<vector::detail::C<i>>;
};

} // namespace arietta::stateful
