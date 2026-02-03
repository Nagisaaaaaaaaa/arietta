#pragma once

/// \file

//
//
//
//
//
#include "Arietta/Types.hpp"

namespace arietta {

template <typename T, usize _rows, usize _cols, is::Types _constants>
class Mat {
public:
  using type = Mat;
  using value_type = T;

  [[nodiscard]] static consteval auto rows() { return _rows; }

  [[nodiscard]] static consteval auto cols() { return _cols; }

  using constants = _constants;

  template <typename... Ts>
  constexpr explicit Mat(Ts &&...) {}
};

//
//
//
namespace is {

namespace detail::mat {

template <typename>
struct IsMat : std::false_type {};

template <typename T, usize rows, usize cols, is::Types constants>
struct IsMat<Mat<T, rows, cols, constants>> : std::true_type {};

} // namespace detail::mat

template <typename T>
concept Mat = detail::mat::IsMat<T>::value;

} // namespace is

namespace isnot {

template <typename T>
concept Mat = !is::Mat<T>;

} // namespace isnot

//
//
//
//
//
// Custom CTAD for `Mat` implemented via compile-time type deduction.
namespace detail::mat {

//! `T` is assumed to be already decayed.
template <typename T>
struct Param;

template <is::Arithmetic T>
struct Param<T> {
  static constexpr bool isMat = false;

  using value_type = T;
  static constexpr usize rows = 1;
  static constexpr usize cols = 1;
  using constants = Types<Types<void>>;
};

template <is::C T>
struct Param<T> {
  static constexpr bool isMat = false;

  using value_type = T::value_type;
  static constexpr usize rows = 1;
  static constexpr usize cols = 1;
  using constants = Types<Types<T>>;
};

template <is::Mat T>
struct Param<T> {
  static constexpr bool isMat = true;

  using value_type = T::value_type;
  static constexpr usize rows = T::rows();
  static constexpr usize cols = T::cols();
  using constants = T::constants;
};

//
//
//
// Policy tags.
struct Row {};

struct Col {};

// Concat rows: `\sum (1, 1) == (n, 1)`.
// Concat cols: `\sum (r, 1) == (r, n)`.
template <typename RowOrCol, typename... Ps>
struct ConcatConstants;

//! `Ps...` are assumed to be non-empty `Param` specializations with size (1, 1).
template <typename... Ps>
struct ConcatConstants<Row, Ps...> {
  using type = Types<Types<typename Ps::constants::template At<0>::template At<0>...>>;
};

//! `Ps...` are assumed to be non-empty `Param` specializations with size (r, 1).
template <typename... Ps>
struct ConcatConstants<Col, Ps...> {
  using type = Types<typename Ps::constants::template At<0>...>;
};

//
//
//
//! `P0` and `Ps...` are assumed to be `Param` specializations.
template <typename P0, typename... Ps>
struct DeduceImpl {
  static_assert(((P0::isMat == Ps::isMat) && ...), "All arguments must be either matrices or non-matrices");

  static_assert(
      (is::Same<typename P0::value_type, typename Ps::value_type> && ...), "All arguments must have the same value type"
  );
  static_assert(((P0::rows == Ps::rows) && ...), "All arguments must have the same number of rows");
  static_assert(P0::cols == 1 && ((Ps::cols == 1) && ...), "All arguments must have exactly one column");

  using value_type = P0::value_type;
  static constexpr usize rows = P0::isMat ? P0::rows : sizeof...(Ps) + 1;
  static constexpr usize cols = P0::isMat ? sizeof...(Ps) + 1 : 1;
  using constants = ConcatConstants<std::conditional_t<P0::isMat, Col, Row>, P0, Ps...>::type;
};

//! `Ts...` are decayed and converted to `Param` specializations.
template <typename... Ts>
struct Deduce : DeduceImpl<Param<std::decay_t<Ts>>...> {};

} // namespace detail::mat

// CTAD.
template <typename... Ts, typename D = detail::mat::Deduce<Ts...>>
Mat(Ts &&...) -> Mat<typename D::value_type, D::rows, D::cols, typename D::constants>;

} // namespace arietta
