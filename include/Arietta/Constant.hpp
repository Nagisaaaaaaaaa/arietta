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

  [[nodiscard]] ART_SPECIFIER constexpr operator value_type() const noexcept { return value; }
};

} // namespace arietta
