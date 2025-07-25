#pragma once

/// \file
/// \brief This file defines a fully‐constexpr "constant wrapper" type that
/// can carry an arbitrary compile‐time value.

//
//
//
//
//
#include "Arietta/Arietta.hpp"

namespace arietta {

template <auto v>
struct C {
  using type = C;
  using value_type = decltype(v);

  static constexpr auto value = v;

  [[nodiscard]] consteval operator value_type() const noexcept { return value; }
};

//
//
//
namespace is {

namespace detail::constant {

template <typename>
struct IsC : std::false_type {};

template <auto v>
struct IsC<C<v>> : std::true_type {};

} // namespace detail::constant

template <typename T>
concept C = detail::constant::IsC<T>::value;

} // namespace is

namespace isnot {

template <typename T>
concept C = !is::C<T>;

} // namespace isnot

//
//
//
//
//
// Operators for `(op C)`, `(C op)`, and `(C op C)`.

#if defined(ART_CONSTANT_LEFT_UNARY_OP) || defined(ART_CONSTANT_RIGHT_UNARY_OP) || defined(ART_CONSTANT_BINARY_OP)
static_assert(false, "Some macros conflict with Arietta");
#endif

#define ART_CONSTANT_LEFT_UNARY_OP(OP)                                                                                 \
  template <auto t>                                                                                                    \
  [[nodiscard]] consteval C<(OP t)> operator OP(C<t>) {                                                                \
    return {};                                                                                                         \
  }
#define ART_CONSTANT_RIGHT_UNARY_OP(OP)                                                                                \
  template <auto t>                                                                                                    \
  [[nodiscard]] consteval C<(t OP)> operator OP(C<t>) {                                                                \
    return {};                                                                                                         \
  }
#define ART_CONSTANT_BINARY_OP(OP)                                                                                     \
  template <auto t, auto u>                                                                                            \
  [[nodiscard]] consteval C<(t OP u)> operator OP(C<t>, C<u>) {                                                        \
    return {};                                                                                                         \
  }

ART_CONSTANT_LEFT_UNARY_OP(+);
ART_CONSTANT_LEFT_UNARY_OP(-);
ART_CONSTANT_LEFT_UNARY_OP(~);
ART_CONSTANT_LEFT_UNARY_OP(!);
ART_CONSTANT_LEFT_UNARY_OP(*);

ART_CONSTANT_BINARY_OP(+);
ART_CONSTANT_BINARY_OP(-);
ART_CONSTANT_BINARY_OP(*);
ART_CONSTANT_BINARY_OP(/);
ART_CONSTANT_BINARY_OP(%);
ART_CONSTANT_BINARY_OP(&);
ART_CONSTANT_BINARY_OP(|);
ART_CONSTANT_BINARY_OP(^);
ART_CONSTANT_BINARY_OP(<<);
ART_CONSTANT_BINARY_OP(>>);

ART_CONSTANT_BINARY_OP(&&);
ART_CONSTANT_BINARY_OP(||);

ART_CONSTANT_BINARY_OP(==);
ART_CONSTANT_BINARY_OP(!=);
ART_CONSTANT_BINARY_OP(>);
ART_CONSTANT_BINARY_OP(<);
ART_CONSTANT_BINARY_OP(>=);
ART_CONSTANT_BINARY_OP(<=);

#undef ART_CONSTANT_LEFT_UNARY_OP
#undef ART_CONSTANT_RIGHT_UNARY_OP
#undef ART_CONSTANT_BINARY_OP

//
//
//
// Operator special cases between `C` and other types.

#if defined(ART_CONSTANT_OP_RETURN)
static_assert(false, "Some macros conflict with Arietta");
#endif

#define ART_CONSTANT_OP_RETURN(EXPR, EXPECT)                                                                           \
  constexpr auto res = (EXPR);                                                                                         \
  static_assert(                                                                                                       \
      static_cast<decltype(EXPECT)>(res) == (EXPECT), "The given type cannot be used in the operators of `C`"          \
  );                                                                                                                   \
  return C<res> {}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] consteval auto operator*(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t * static_cast<std::decay_t<U>>(1), 0);
}

template <typename U, auto t>
  requires(t == 0)
[[nodiscard]] consteval auto operator*(U &&, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<std::decay_t<U>>(1) * t, 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] consteval auto operator/(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t / static_cast<std::decay_t<U>>(1), 0);
}

template <typename U, auto t>
  requires(t == 1 || t == -1)
[[nodiscard]] consteval auto operator%(U &&, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<std::decay_t<U>>(1) % t, 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] consteval auto operator%(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t % static_cast<std::decay_t<U>>(1), 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] consteval auto operator&(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t & static_cast<std::decay_t<U>>(1), 0);
}

template <typename U, auto t>
  requires(t == 0)
[[nodiscard]] consteval auto operator&(U &&, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<std::decay_t<U>>(1) & t, 0);
}

template <auto t, typename U>
  requires(!static_cast<bool>(t))
[[nodiscard]] consteval auto operator&&(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t && static_cast<std::decay_t<U>>(true), false);
}

template <typename U, auto t>
  requires(!static_cast<bool>(t))
[[nodiscard]] consteval auto operator&&(U &&, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<std::decay_t<U>>(true) && t, false);
}

template <auto t, typename U>
  requires(static_cast<bool>(t))
[[nodiscard]] consteval auto operator||(C<t>, U &&) {
  ART_CONSTANT_OP_RETURN(t || static_cast<std::decay_t<U>>(false), true);
}

template <typename U, auto t>
  requires(static_cast<bool>(t))
[[nodiscard]] consteval auto operator||(U &&, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<std::decay_t<U>>(false) || t, true);
}

#undef ART_CONSTANT_OP_RETURN

//
//
//
//
//
namespace detail::constant {

template <auto i, auto n, auto one, typename F>
ART_SPECIFIER constexpr void ForEachImpl(F &&f) {
  if constexpr (i < n) {
    if constexpr (is::Invocable<F, C<i>>) {
      static_assert(is::Void<decltype(f(C<i>{}))>, "Function must have void return type");
      f(C<i>{});
    } else {
      static_assert(is::Void<decltype(f.template operator()<i>())>, "Function must have void return type");
      f.template operator()<i>();
    }

    ForEachImpl<i + one, n, one>(std::forward<F>(f));
  }
}

} // namespace detail::constant

template <auto n, typename F>
  requires is::Arithmetic<decltype(n)>
ART_SPECIFIER constexpr void ForEach(C<n>, F &&f) {
  using V = decltype(n);
  detail::constant::ForEachImpl<static_cast<V>(0), n, static_cast<V>(1)>(std::forward<F>(f));
}

template <auto n, typename F>
  requires is::Arithmetic<decltype(n)>
ART_SPECIFIER constexpr void ForEach(F &&f) {
  ForEach(C<n>{}, std::forward<F>(f));
}

template <is::C N, typename F>
  requires is::Arithmetic<typename N::value_type>
ART_SPECIFIER constexpr void ForEach(F &&f) {
  ForEach(N{}, std::forward<F>(f));
}

} // namespace arietta
