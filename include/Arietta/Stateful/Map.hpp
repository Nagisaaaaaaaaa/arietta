#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace map::detail {

template <typename T, typename Key>
struct Reader {
  friend auto flag(Reader<T, Key>);
};

template <typename Value>
struct Wrapper {
  using type = Value;
};

template <typename T, typename Key, typename Value>
struct Setter {
  friend auto flag(Reader<T, Key>) { return Wrapper<Value>{}; }
};

struct Default {};

} // namespace map::detail

template <typename T = map::detail::Default>
struct Map {
  template <typename Key, typename Value>
  static consteval void Insert() {
    [[maybe_unused]] map::detail::Setter<T, Key, Value> s;
  }

  template <typename Key>
  using Find = typename decltype(flag(map::detail::Reader<T, Key>{}))::type;
};

} // namespace arietta::stateful
