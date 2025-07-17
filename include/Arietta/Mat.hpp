#pragma once

#include "Arietta/Types.hpp"

#include <array>
#include <limits>

namespace arietta {

template <typename T, usize row, usize col, is::Types Uss>
class Mat {
public:
  static_assert([]() {
    bool res = true;
    res = res && (Uss::Size() == row);
    ForEach<Uss>([&]<is::Types Us>() {
      res = res && (Us::Size() == col);
      ForEach<Us>([&]<typename U>() {
        res = res && (is::Same<U, void> || is::C<U>);
        if constexpr (is::C<U>)
          res = res && is::Same<typename U::value_type, T>;
      });
    });
    return res;
  }());

  static consteval std::array<std::array<usize, col>, row> coords2Indices() {
    std::array<std::array<usize, col>, row> res{};
    usize i = 0;
    ForEach<row>([&]<auto r>() {
      using Us = typename Uss::template At<r>;
      ForEach<col>([&]<auto c>() {
        using U = typename Us::template At<c>;
        res[r][c] = is::Same<U, void> ? i++ : std::numeric_limits<usize>::max();
      });
    });
    return res;
  }

  static consteval usize nValuesToStore() {
    usize n = 0;
    ForEach<row>([&]<auto r>() {
      using Us = typename Uss::template At<r>;
      ForEach<col>([&]<auto c>() {
        using U = typename Us::template At<c>;
        if constexpr (is::Same<U, void>)
          ++n;
      });
    });
    return n;
  }

  std::array<T, nValuesToStore()> storage_;
};

} // namespace arietta
