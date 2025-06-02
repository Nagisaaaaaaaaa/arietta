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

#if defined(ART_LEFT_UNARY_OP) || defined(ART_RIGHT_UNARY_OP) || defined(ART_BINARY_OP)
static_assert(false, "Some macros conflict with Arietta");
#endif

#define ART_LEFT_UNARY_OP(OP)                                                                                          \
  template <auto t>                                                                                                    \
  [[nodiscard]] ART_SPECIFIER consteval C<(OP t)> operator OP(C<t>) {                                                  \
    return {};                                                                                                         \
  }
#define ART_RIGHT_UNARY_OP(OP)                                                                                         \
  template <auto t>                                                                                                    \
  [[nodiscard]] ART_SPECIFIER consteval C<(t OP)> operator OP(C<t>) {                                                  \
    return {};                                                                                                         \
  }
#define ART_BINARY_OP(OP)                                                                                              \
  template <auto t, auto u>                                                                                            \
  [[nodiscard]] ART_SPECIFIER consteval C<(t OP u)> operator OP(C<t>, C<u>) {                                          \
    return {};                                                                                                         \
  }

ART_LEFT_UNARY_OP(+);
ART_LEFT_UNARY_OP(-);
ART_LEFT_UNARY_OP(~);
ART_LEFT_UNARY_OP(!);
ART_LEFT_UNARY_OP(*);

ART_BINARY_OP(+);
ART_BINARY_OP(-);
ART_BINARY_OP(*);
ART_BINARY_OP(/);
ART_BINARY_OP(%);
ART_BINARY_OP(&);
ART_BINARY_OP(|);
ART_BINARY_OP(^);
ART_BINARY_OP(<<);
ART_BINARY_OP(>>);

ART_BINARY_OP(&&);
ART_BINARY_OP(||);

ART_BINARY_OP(==);
ART_BINARY_OP(!=);
ART_BINARY_OP(>);
ART_BINARY_OP(<);
ART_BINARY_OP(>=);
ART_BINARY_OP(<=);

#undef ART_LEFT_UNARY_OP
#undef ART_RIGHT_UNARY_OP
#undef ART_BINARY_OP

} // namespace arietta
