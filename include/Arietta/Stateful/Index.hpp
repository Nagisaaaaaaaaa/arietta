#pragma once

/// \file
/// \brief This file implements a compile-time counter, `Index`.
///
/// \warning This implementation relies on stateful template metaprogramming,
/// which may lead to unexpected behavior if misused or
/// relied upon across translation units.
///
/// \code
/// static_assert(Index<>::Load() == 0);
/// static_assert(Index<>::FetchAdd() == 0);
/// static_assert(Index<>::Load() == 1);
/// static_assert(Index<>::FetchAdd() == 1);
/// static_assert(Index<>::FetchAdd() == 2);
/// static_assert(Index<>::Load() == 3);
///
/// static_assert(Index<T>::Load() == 0);
/// static_assert(Index<T>::FetchAdd() == 0);
/// static_assert(Index<T>::Load() == 1);
/// static_assert(Index<T>::FetchAdd() == 1);
/// static_assert(Index<T>::FetchAdd() == 2);
/// static_assert(Index<T>::Load() == 3);
/// \endcode

//
//
//
//
//
#include "Arietta/Arietta.hpp"

namespace arietta::stateful {
namespace {

namespace detail::index {

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

template <typename>
struct Anon {};

template <auto>
struct C {};

} // namespace detail::index

//
//
//
// TODO: Currently, template parameters in C++ classes can only be either `typename` or `auto`.
//       In other words, they do not truly support overloading in the same way functions do.
//       As a result, the template parameters of `Index`, `Map`, etc. and their members
//       currently support only `typename`, not `auto`.
template <typename _ = detail::index::Default, typename T = detail::index::Anon<_>>
struct Index {
  template <auto tag = []() {}>
  [[nodiscard]] static consteval auto Load() {
    return detail::index::LoadImpl<T, tag>();
  };

  template <auto tag = []() {}>
  static consteval auto FetchAdd() {
    return detail::index::FetchAddImpl<T, tag>();
  };
};

} // namespace
} // namespace arietta::stateful
