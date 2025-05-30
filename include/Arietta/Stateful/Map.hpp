#pragma once

#include "Arietta/Arietta.hpp"

namespace arietta::stateful {

namespace map::detail {

template <typename T, typename Key>
struct Reader {
  friend consteval auto flag(Reader<T, Key>);
};

template <typename Value>
struct Wrapper {
  using type = Value;
};

template <typename Value>
struct Dummy {};

template <typename T, typename Key, typename Value>
struct Setter {
  friend consteval auto flag(Reader<T, Key>) { return Wrapper<Value>{}; }

  static constexpr auto value = Dummy<Value>{};
};

struct Default {};

template <auto v>
struct C {};

} // namespace map::detail

// TODO: Currently, template parameters in C++ classes can only be either `typename` or `auto`.
//       In other words, they do not truly support overloading in the same way functions do.
//       As a result, the template parameters of `class Map` and `At`
//       currently support only `typename`, not `auto`.
template <typename T = map::detail::Default>
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

} // namespace arietta::stateful
