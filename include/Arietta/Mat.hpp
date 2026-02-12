#pragma once

/// \file

//
//
//
//
//
#include "Arietta/Types.hpp"

namespace arietta {

namespace detail::mat {

//! The last template parameter of `Mat` is an instance of `Token`.
//! This design aims to strictly prohibit users from manually spelling out the `Mat` type.
//! The rationale is as follows:
//! 1. Users should never need to explicitly write the exact type of `Mat`.
//! 2. It is difficult for users to correctly specify the `Mat` type due to the complexity of `Constants`.
//!    Validating `Constants` upon every instantiation would impose a heavy burden on compilation speed.
//!    The token achieves the validation goal with minimal compile-time overhead:
//!    the presence of a token guarantees that `Constants` is valid.
//!    Consequently, we can confine all validity checks to the `Deduce` phase.
class Token {
  //! `private` prevents users from manually constructing `Token` externally.
private:
  Token() = default;

  template <typename...>
  friend struct Deduce;
};

} // namespace detail::mat

//
//
//
template <typename T, typename _Constants, detail::mat::Token>
class Mat {
public:
  using type = Mat;
  using value_type = T;
  using Constants = _Constants;

  [[nodiscard]] static consteval auto rows() { return Constants::template At<0>::Size(); }

  [[nodiscard]] static consteval auto cols() { return Constants::Size(); }

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

template <typename T, typename Constants, ::arietta::detail::mat::Token token>
struct IsMat<Mat<T, Constants, token>> : std::true_type {};

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

// CRTP base providing `rows()` and `cols()` for `Param`.
template <typename Derived>
struct ParamBase {
  [[nodiscard]] static consteval auto rows() { return Derived::Constants::template At<0>::Size(); }

  [[nodiscard]] static consteval auto cols() { return Derived::Constants::Size(); }
};

template <is::Arithmetic T>
struct Param<T> : ParamBase<Param<T>> {
  static constexpr bool isMat = false;
  using value_type = T;
  using Constants = Types<Types<void>>;
};

template <is::C T>
struct Param<T> : ParamBase<Param<T>> {
  static constexpr bool isMat = false;
  using value_type = T::value_type;
  using Constants = Types<Types<T>>;
};

template <is::Mat T>
struct Param<T> : ParamBase<Param<T>> {
  static constexpr bool isMat = true;
  using value_type = T::value_type;
  using Constants = T::Constants;
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
  using type = Types<Types<typename Ps::Constants::template At<0>::template At<0>...>>;
};

//! `Ps...` are assumed to be non-empty `Param` specializations with size (r, 1).
template <typename... Ps>
struct ConcatConstants<Col, Ps...> {
  using type = Types<typename Ps::Constants::template At<0>...>;
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
  static_assert(((P0::rows() == Ps::rows()) && ...), "All arguments must have the same number of rows");
  static_assert(P0::cols() == 1 && ((Ps::cols() == 1) && ...), "All arguments must have exactly one column");

  using value_type = P0::value_type;
  using Constants = ConcatConstants<std::conditional_t<P0::isMat, Col, Row>, P0, Ps...>::type;
};

//! `Ts...` are decayed and converted to `Param` specializations.
template <typename... Ts>
struct Deduce : DeduceImpl<Param<std::decay_t<Ts>>...> {
  static constexpr Token token{};
};

} // namespace detail::mat

// CTAD.
template <typename... Ts, typename D = detail::mat::Deduce<Ts...>>
Mat(Ts &&...) -> Mat<typename D::value_type, typename D::Constants, D::token>;

} // namespace arietta
