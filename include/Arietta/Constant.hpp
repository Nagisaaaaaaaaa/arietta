#pragma once

/// \file
/// \brief This file declares a fully‐constexpr "constant wrapper" type that
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

  [[nodiscard]] ART_SPECIFIER consteval operator value_type() const noexcept { return value; }
};

//
//
//
// Operators for `(op C)`, `(C op)`, and `(C op C)`.

#if defined(ART_CONSTANT_LEFT_UNARY_OP) || defined(ART_CONSTANT_RIGHT_UNARY_OP) || defined(ART_CONSTANT_BINARY_OP)
static_assert(false, "Some macros conflict with Arietta");
#endif

#define ART_CONSTANT_LEFT_UNARY_OP(OP)                                                                                 \
  template <auto t>                                                                                                    \
  [[nodiscard]] ART_SPECIFIER consteval C<(OP t)> operator OP(C<t>) {                                                  \
    return {};                                                                                                         \
  }
#define ART_CONSTANT_RIGHT_UNARY_OP(OP)                                                                                \
  template <auto t>                                                                                                    \
  [[nodiscard]] ART_SPECIFIER consteval C<(t OP)> operator OP(C<t>) {                                                  \
    return {};                                                                                                         \
  }
#define ART_CONSTANT_BINARY_OP(OP)                                                                                     \
  template <auto t, auto u>                                                                                            \
  [[nodiscard]] ART_SPECIFIER consteval C<(t OP u)> operator OP(C<t>, C<u>) {                                          \
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
[[nodiscard]] ART_SPECIFIER consteval auto operator*(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t * static_cast<U>(1), 0);
}

template <typename U, auto t>
  requires(t == 0)
[[nodiscard]] ART_SPECIFIER consteval auto operator*(U, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<U>(1) * t, 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] ART_SPECIFIER consteval auto operator/(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t / static_cast<U>(1), 0);
}

template <typename U, auto t>
  requires(t == 1 || t == -1)
[[nodiscard]] ART_SPECIFIER consteval auto operator%(U, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<U>(1) % t, 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] ART_SPECIFIER consteval auto operator%(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t % static_cast<U>(1), 0);
}

template <auto t, typename U>
  requires(t == 0)
[[nodiscard]] ART_SPECIFIER consteval auto operator&(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t & static_cast<U>(1), 0);
}

template <typename U, auto t>
  requires(t == 0)
[[nodiscard]] ART_SPECIFIER consteval auto operator&(U, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<U>(1) & t, 0);
}

template <auto t, typename U>
  requires(!static_cast<bool>(t))
[[nodiscard]] ART_SPECIFIER consteval auto operator&&(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t && static_cast<U>(true), false);
}

template <typename U, auto t>
  requires(!static_cast<bool>(t))
[[nodiscard]] ART_SPECIFIER consteval auto operator&&(U, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<U>(true) && t, false);
}

template <auto t, typename U>
  requires(static_cast<bool>(t))
[[nodiscard]] ART_SPECIFIER consteval auto operator||(C<t>, U) {
  ART_CONSTANT_OP_RETURN(t || static_cast<U>(false), true);
}

template <typename U, auto t>
  requires(static_cast<bool>(t))
[[nodiscard]] ART_SPECIFIER consteval auto operator||(U, C<t>) {
  ART_CONSTANT_OP_RETURN(static_cast<U>(false) || t, true);
}

#undef ART_CONSTANT_OP_RETURN

} // namespace arietta
