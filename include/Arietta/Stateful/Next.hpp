#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace next::detail {

template <usize n>
struct Reader {
  friend auto flag(Reader<n>);
};

template <usize n>
struct Setter {
  friend auto flag(Reader<n>) {}

  static constexpr usize value = n;
};

template <auto tag, usize i = 0>
[[nodiscard]] consteval auto NextImpl() {
  if constexpr (requires(Reader<i> r) { flag(r); }) {
    return NextImpl<tag, i + 1>();
  } else {
    Setter<i> s;
    return s.value;
  }
}

} // namespace next::detail

template <auto tag = []() {}, auto res = next::detail::NextImpl<tag>()>
consteval auto Next() {
  return res;
};

} // namespace arietta::stateful
