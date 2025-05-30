#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace map::detail {

template <typename Key>
struct Reader {
  friend auto flag(Reader<Key>);
};

template <typename Value>
struct Wrapper {
  using type = Value;
};

template <typename Key, typename Value>
struct Setter {
  friend auto flag(Reader<Key>) { return Wrapper<Value>{}; }
};

} // namespace map::detail

template <typename Key, typename Value>
consteval void Insert() {
  map::detail::Setter<Key, Value> s;
}

template <typename Key>
using Find = typename decltype(flag(map::detail::Reader<Key>{}))::type;

} // namespace arietta::stateful
