#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace next::detail {

template <typename T, usize i>
struct Reader {
  friend auto flag(Reader<T, i>);
};

template <typename T, usize i>
struct Setter {
  friend auto flag(Reader<T, i>) {}

  static constexpr usize value = i;
};

template <typename T, auto tag, usize i = 0>
[[nodiscard]] consteval auto NextImpl() {
  if constexpr (requires(Reader<T, i> r) { flag(r); }) {
    return NextImpl<T, tag, i + 1>();
  } else {
    Setter<T, i> s;
    return s.value;
  }
}

struct Default {};

template <auto v>
struct C {};

} // namespace next::detail

template <typename T = next::detail::Default, auto tag = []() {}>
consteval auto Next() {
  return next::detail::NextImpl<T, tag>();
};

template <auto v, auto tag = []() {}>
consteval auto Next() {
  return Next<next::detail::C<v>, tag>();
};

} // namespace arietta::stateful
