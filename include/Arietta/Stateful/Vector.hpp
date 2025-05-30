#pragma once

#include "Arietta/Stateful/Map.hpp"
#include "Arietta/Stateful/Next.hpp"

namespace arietta::stateful {

namespace vector::detail {

struct Default {};

template <auto v>
struct C {};

} // namespace vector::detail

template <typename T = vector::detail::Default>
struct Vector {
  template <typename Value, auto tag = []() {}>
  static consteval void PushBack() {
    constexpr usize i = Next<T, tag>();
    Map<T>::template Insert<vector::detail::C<i>, Value>();
  }

  template <usize i>
  using At = typename Map<T>::template At<vector::detail::C<i>>;
};

} // namespace arietta::stateful
