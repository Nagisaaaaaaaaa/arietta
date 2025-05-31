#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace index::detail {

template <typename T, usize i>
struct Reader {
  friend consteval auto flag(Reader<T, i>);
};

template <typename T, usize i>
struct Setter {
  friend consteval auto flag(Reader<T, i>) {}

  static constexpr usize value = i;
};

template <typename T, auto tag, usize i = 0>
[[nodiscard]] consteval auto LoadImpl() {
  if constexpr (requires(Reader<T, i> r) { flag(r); })
    return LoadImpl<T, tag, i + 1>();
  else
    return i;
}

template <typename T, auto tag, usize i = 0>
[[nodiscard]] consteval auto FetchAddImpl() {
  if constexpr (requires(Reader<T, i> r) { flag(r); }) {
    return FetchAddImpl<T, tag, i + 1>();
  } else {
    Setter<T, i> s;
    return s.value;
  }
}

struct Default {};

template <auto v>
struct C {};

} // namespace index::detail

template <typename T = index::detail::Default, auto tag = []() {}>
consteval auto Load() {
  return index::detail::LoadImpl<T, tag>();
};

template <auto v, auto tag = []() {}>
consteval auto Load() {
  return Load<index::detail::C<v>, tag>();
};

template <typename T = index::detail::Default, auto tag = []() {}>
consteval auto FetchAdd() {
  return index::detail::FetchAddImpl<T, tag>();
};

template <auto v, auto tag = []() {}>
consteval auto FetchAdd() {
  return FetchAdd<index::detail::C<v>, tag>();
};

} // namespace arietta::stateful
