#pragma once

/// \file

//
//
//
//
//
#include "Arietta/Mat.hpp"

namespace arietta::space_filling_curve {

template <usize dim>
struct Morton;

template <>
struct Morton<1> {
  template <is::Integral I, typename... Ts>
  [[nodiscard]] ART_SPECIFIER constexpr auto operator()(Vec1<I, Ts...> const &crd) {
    return crd(static_cast<usize>(0));
  }

  template <typename I>
    requires(is::Integral<I> || (is::C<I> && is::Integral<typename I::value_type>))
  [[nodiscard]] ART_SPECIFIER constexpr auto operator()(I const &v) {
    return Vec{v};
  }
};

} // namespace arietta::space_filling_curve
