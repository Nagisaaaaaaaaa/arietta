#pragma once

/// \file

//
//
//
//
//
#include "Arietta/Types.hpp"

#include <array>

namespace arietta {

template <typename T, usize rows, usize cols, typename... Ts>
class Mat;

template <typename T, usize rows, typename... Ts>
using Vec = Mat<T, rows, 1, Ts...>;

namespace is {

namespace detail::mat {

template <typename>
struct IsMat : std::false_type {};

//! Only a `Mat` with specified `Constants` and `Token` is considered `is::Mat`.
template <typename T, usize rows, usize cols, typename Constants, typename Token>
struct IsMat<Mat<T, rows, cols, Constants, Token>> : std::true_type {};

template <typename>
struct IsVec : std::false_type {};

//! Only a `Vec` with specified `Constants` and `Token` is considered `is::Vec`.
template <typename T, usize rows, typename Constants, typename Token>
struct IsVec<Vec<T, rows, Constants, Token>> : std::true_type {};

} // namespace detail::mat

template <typename T>
concept Mat = detail::mat::IsMat<T>::value;

template <typename T>
concept Vec = detail::mat::IsVec<T>::value;

} // namespace is

namespace isnot {

template <typename T>
concept Mat = !is::Mat<T>;

template <typename T>
concept Vec = !is::Vec<T>;

} // namespace isnot

//
//
//
namespace detail::mat {

#if 1 // TODO: Bypass NVCC bugs.
template <typename... Ts>
struct Deduce;
#endif

//! The last optional template parameter of `Mat` is an instance of `Token`, wrapped in `C`.
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
#if 0 // TODO: Bypass NVCC bugs.
private:
#else
public:
#endif
  Token() = default;

  template <typename T, usize rows, usize cols, typename... Ts>
  friend class arietta::Mat;

  template <typename... Ts>
#if 0 // TODO: Bypass NVCC bugs.
    requires(sizeof...(Ts) > 0)
#endif
  friend struct Deduce;
};

//
//
//
template <typename T, usize _rows, usize _cols>
class MatBase {
public:
  using type = MatBase;
  using value_type = T;

  [[nodiscard]] static constexpr auto rows() { return _rows; }

  [[nodiscard]] static constexpr auto cols() { return _cols; }
};

template <typename Constants>
[[nodiscard]] consteval bool HasStorage() {
  bool res = false;
  ForEach<Constants>([&]<typename ConstantsPerCol>() {
    ForEach<ConstantsPerCol>([&]<typename Constant>() {
      if constexpr (is::Same<Constant, void>)
        res = true;
    });
  });
  return res;
}

template <typename T, typename Constants>
class MatStorage {
public:
  using Storage = MatStorage;

  constexpr void storage() const;

  constexpr void storage();
};

template <typename T, typename Constants>
  requires(HasStorage<Constants>())
class MatStorage<T, Constants> {
public:
  using Storage = MatStorage;

  [[nodiscard]] ART_SPECIFIER constexpr auto const &storage() const { return storage_; }

  [[nodiscard]] ART_SPECIFIER constexpr auto &storage() { return storage_; }

private:
  [[nodiscard]] static consteval usize storageSize() {
    usize res = 0;
    ForEach<Constants>([&]<typename ConstantsPerCol>() {
      ForEach<ConstantsPerCol>([&]<typename Constant>() {
        if constexpr (is::Same<Constant, void>)
          ++res;
      });
    });
    return res;
  }

  std::array<T, storageSize()> storage_;
};

//
//
//
//
//
// Custom CTAD for `Mat` implemented via compile-time type deduction.

//! `T` is assumed to be already decayed.
template <typename T>
struct Param;

// CRTP base providing `rows()` and `cols()` for `Param`.
template <typename Derived>
struct ParamBase {
  [[nodiscard]] static constexpr auto rows() { return Derived::Constants::template At<0>::Size(); }

  [[nodiscard]] static constexpr auto cols() { return Derived::Constants::Size(); }
};

template <typename T>
  requires(!is::C<T> && !is::Mat<T>)
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

  using type = DeduceImpl;
  using value_type = P0::value_type;
  using Constants = ConcatConstants<std::conditional_t<P0::isMat, Col, Row>, P0, Ps...>::type;
};

// Specialization for copy and move constructors deduction.
//! `P` is assumed to be a `Param` specialization.
template <typename P>
  requires(P::isMat)
struct DeduceImpl<P> {
  using type = DeduceImpl;
  using value_type = P::value_type;
  using Constants = P::Constants;
};

//! `Ts...` are decayed and converted to `Param` specializations.
#if 0 // TODO: Bypass NVCC bugs.
template <typename... Ts>
  requires(sizeof...(Ts) > 0)
struct Deduce : DeduceImpl<Param<std::decay_t<Ts>>...> {
#else
template <typename T0, typename... Ts>
struct Deduce<T0, Ts...> : DeduceImpl<Param<std::decay_t<T0>>, Param<std::decay_t<Ts>>...> {
#endif
  using Base = typename Deduce::type;

  using type = Deduce;
  using value_type = Base::value_type;
  using Constants = Base::Constants;

  static constexpr usize rows = Constants::template At<0>::Size();
  static constexpr usize cols = Constants::Size();
  static constexpr Token token{};
};

} // namespace detail::mat

//
//
//
//
//
template <typename T, usize _rows, usize _cols, typename _Constants, typename Token>
//! The token constraint should be expressed via `requires` instead of `static_assert`,
//! since `requires` participates in substitution failure,
//! while `static_assert` is only evaluated during instantiation.
  requires(is::C<Token> && is::Same<typename Token::value_type, detail::mat::Token>)
class Mat<T, _rows, _cols, _Constants, Token> : public detail::mat::MatBase<T, _rows, _cols>,
                                                public detail::mat::MatStorage<T, _Constants> {
  //! static_assert(is::C<Token> && is::Same<typename Token::value_type, detail::mat::Token>);
private:
  using Base = typename Mat::type;
  using Mat::Storage::storage;

public:
  using type = Mat;
  using Constants = _Constants;

  using Base::rows, Base::cols;
  using typename Base::value_type;

  Mat() = default;

  template <typename... Us, typename D = detail::mat::Deduce<Us...>>
    requires(
        is::Same<Mat, Mat<typename D::value_type, D::rows, D::cols, typename D::Constants, C<D::token>>> &&
        is::Mat<std::decay_t<typename Types<Us...>::template At<0>>> &&
        sizeof...(Us) > 1 //! Avoid conflicting with copy and move constructors.
    )
  ART_SPECIFIER constexpr explicit Mat(Us &&...us) {
    auto init = [&]<usize col, typename V, typename... Vs>(auto &&self, V &&v, Vs &&...vs) constexpr {
      ForEach<rows()>([&]<auto row>() { this->operator()(C<row>{}, C<col>{}) = v(C<row>{}); });

      if constexpr (sizeof...(Vs) > 0)
        self.template operator()<col + 1>(self, std::forward<Vs>(vs)...);
    };

    init.template operator()<0>(init, std::forward<Us>(us)...);
  }

  template <typename... Us, typename D = detail::mat::Deduce<Us...>>
    requires(
        is::Same<Mat, Mat<typename D::value_type, D::rows, D::cols, typename D::Constants, C<D::token>>> &&
        isnot::Mat<std::decay_t<typename Types<Us...>::template At<0>>>
    )
  ART_SPECIFIER constexpr explicit Mat(Us &&...us) {
    auto init = [&]<usize row, typename V, typename... Vs>(auto &&self, V &&v, Vs &&...vs) constexpr {
      this->operator()(C<row>{}, C<static_cast<usize>(0)>{}) = std::forward<V>(v);

      if constexpr (sizeof...(Vs) > 0)
        self.template operator()<row + 1>(self, std::forward<Vs>(vs)...);
    };

    init.template operator()<0>(init, std::forward<Us>(us)...);
  }

  template <
      typename... Us,
      typename D = detail::mat::Deduce<Us...>,
      typename M = Mat<typename D::value_type, D::rows, D::cols, typename D::Constants, C<D::token>>>
    requires(isnot::Same<Mat, M>)
  ART_SPECIFIER constexpr explicit Mat(Us &&...us) {
    M m{std::forward<Us>(us)...};

    ForEach<cols()>([&]<auto col>() {
      ForEach<rows()>([&]<auto row>() { operator()(C<row>{}, C<col>{}) = m(C<row>{}, C<col>{}); });
    });
  }

  template <typename M>
    requires(is::Mat<std::decay_t<M>> && isnot::Same<Mat, std::decay_t<M>>)
  ART_SPECIFIER constexpr Mat &operator=(M &&m) {
    return *this = Mat{std::forward<M>(m)};
  }

public:
  template <auto row, auto col>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(C<row>, C<col>) const {
    if constexpr (is::Same<Constant<row, col>, void>)
      return storage()[storageIdx<row, col>()];
    else
      return Constant<row, col>{};
  }

  template <auto row, auto col>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(C<row>, C<col>) {
    if constexpr (is::Same<Constant<row, col>, void>)
      return storage()[storageIdx<row, col>()];
    else
      return Constant<row, col>{};
  }

  template <typename tag = void>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(usize row, usize col) const {
    static_assert(
        is::Same<Constants, Types<>::Fill<Types<>::Fill<void, rows()>, cols()>>,
        "Runtime row-column access is only available for fully stored matrices"
    );
    return storage()[row + col * C<rows()>{}];
  }

  template <typename tag = void>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(usize row, usize col) {
    static_assert(
        is::Same<Constants, Types<>::Fill<Types<>::Fill<void, rows()>, cols()>>,
        "Runtime row-column access is only available for fully stored matrices"
    );
    return storage()[row + col * C<rows()>{}];
  }

  template <auto row>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(C<row>) const {
    static_assert(cols() == 1, "Single-index access is only available for column vectors");
    return operator()(C<row>{}, C<static_cast<usize>(0)>{});
  }

  template <auto row>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(C<row>) {
    static_assert(cols() == 1, "Single-index access is only available for column vectors");
    return operator()(C<row>{}, C<static_cast<usize>(0)>{});
  }

  template <typename tag = void>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(usize row) const {
    static_assert(cols() == 1, "Single-index access is only available for column vectors");
    return operator()<tag>(row, static_cast<usize>(0));
  }

  template <typename tag = void>
  [[nodiscard]] ART_SPECIFIER constexpr decltype(auto) operator()(usize row) {
    static_assert(cols() == 1, "Single-index access is only available for column vectors");
    return operator()<tag>(row, static_cast<usize>(0));
  }

private:
  template <usize row, usize col>
  using Constant = typename Constants::template At<col>::template At<row>;

  template <usize row, usize col>
  [[nodiscard]] static consteval usize storageIdx() {
    //! `row` and `col` are assumed to satisfy `row < rows()` and `col < cols()`.

    usize res = 0;

    ForEach<col>([&]<auto c>() {
      ForEach<rows()>([&]<auto r>() {
        if constexpr (is::Same<Constant<r, c>, void>)
          ++res;
      });
    });

    ForEach<row>([&]<auto r>() {
      if constexpr (is::Same<Constant<r, col>, void>)
        ++res;
    });

    return res;
  }
};

//
//
//
template <typename T, usize _rows, usize _cols>
class Mat<T, _rows, _cols> : public detail::mat::MatBase<T, _rows, _cols> {
private:
  using Base = typename Mat::type;
  static constexpr detail::mat::Token token{};

public:
  using type = Mat;

  using Base::rows, Base::cols;
  using typename Base::value_type;

  Mat() = delete;

public:
  using AsStorage = Mat<T, rows(), cols(), Types<>::Fill<Types<>::Fill<void, rows()>, cols()>, C<token>>;

  template <T v>
  [[nodiscard]] static consteval auto Constant() {
    return Constant<C<v>>();
  }

  template <is::C V>
    requires is::Same<typename V::value_type, T>
  [[nodiscard]] static consteval auto Constant() {
    using ConstantsPerCol = Types<>::Fill<V, rows()>;
    using Constants = Types<>::Fill<ConstantsPerCol, cols()>;
    return Mat<T, rows(), cols(), Constants, C<token>>{};
  }

  [[nodiscard]] static consteval auto Zero() { return Constant<static_cast<T>(0)>(); }

  [[nodiscard]] static consteval auto Identity() {
    static_assert(rows() == cols(), "The identity matrix is only defined for square matrices");
    using Constants = Types<>::FillEach<IdentityConstantsPerCol, cols()>;
    return Mat<T, rows(), cols(), Constants, C<token>>{};
  }

private:
  template <usize col>
  struct IdentityConstantsPerElement {
    template <usize row>
    using type = std::conditional_t<row == col, C<static_cast<T>(1)>, C<static_cast<T>(0)>>;
  };

  template <usize col>
  using IdentityConstantsPerCol = Types<>::FillEach<IdentityConstantsPerElement<col>::template type, rows()>;
};

//
//
//
// CTAD.
template <typename... Ts, typename D = detail::mat::Deduce<Ts...>>
Mat(Ts &&...) -> Mat<typename D::value_type, D::rows, D::cols, typename D::Constants, C<D::token>>;

//
//
//
//
//
// Operators.
namespace detail::mat {

//! `M` is assumed to satisfy `is::Mat`.
template <auto op, usize col, typename M, usize... row>
[[nodiscard]] ART_SPECIFIER constexpr auto OpUnaryImplPerCol(M const &m, std::index_sequence<row...>) {
  return Mat{op(m(C<row>{}, C<col>{}))...};
}

template <auto op, typename M, usize... col>
[[nodiscard]] ART_SPECIFIER constexpr auto OpUnaryImpl(M const &m, std::index_sequence<col...>) {
  return Mat{OpUnaryImplPerCol<op, col>(m, std::make_index_sequence<M::rows()>{})...};
}

template <auto op, typename M>
[[nodiscard]] ART_SPECIFIER constexpr auto OpUnary(M const &m) {
  return OpUnaryImpl<op>(m, std::make_index_sequence<M::cols()>{});
}

//! If both `Lhs` and `Rhs` satisfy `is::Mat`, they are assumed to have the same rows and columns,
//! which are already passed as template parameters.
template <auto op, usize col, typename Lhs, typename Rhs, usize... row>
[[nodiscard]] ART_SPECIFIER constexpr auto
OpBinaryImplPerCol(Lhs const &lhs, Rhs const &rhs, std::index_sequence<row...>) {
  if constexpr (is::Mat<Lhs> && is::Mat<Rhs>)
    return Mat{op(lhs(C<row>{}, C<col>{}), rhs(C<row>{}, C<col>{}))...};
  else if constexpr (!is::Mat<Lhs> && is::Mat<Rhs>)
    return Mat{op(lhs, rhs(C<row>{}, C<col>{}))...};
  else if constexpr (is::Mat<Lhs> && !is::Mat<Rhs>)
    return Mat{op(lhs(C<row>{}, C<col>{}), rhs)...};
}

template <auto op, usize rows, typename Lhs, typename Rhs, usize... col>
[[nodiscard]] ART_SPECIFIER constexpr auto OpBinaryImpl(Lhs const &lhs, Rhs const &rhs, std::index_sequence<col...>) {
  return Mat{OpBinaryImplPerCol<op, col>(lhs, rhs, std::make_index_sequence<rows>{})...};
}

template <auto op, usize rows, usize cols, typename Lhs, typename Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto OpBinary(Lhs const &lhs, Rhs const &rhs) {
  return OpBinaryImpl<op, rows>(lhs, rhs, std::make_index_sequence<cols>{});
}

} // namespace detail::mat

//
//
//
// TODO: The current operator design follows integral promotion,
//       so integer types narrower than `int` are promoted during computation.
template <is::Mat M>
[[nodiscard]] ART_SPECIFIER constexpr auto operator+(M const &m) {
  return detail::mat::OpUnary<[](auto const &v) { return +v; }>(m);
}

template <is::Mat M>
[[nodiscard]] ART_SPECIFIER constexpr auto operator-(M const &m) {
  return detail::mat::OpUnary<[](auto const &v) { return -v; }>(m);
}

template <is::Mat Lhs, is::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr bool operator==(Lhs const &lhs, Rhs const &rhs) {
  static_assert(
      Lhs::rows() == Rhs::rows() && Lhs::cols() == Rhs::cols(),
      "Binary operators are only defined for matrices with the same dimensions"
  );

  bool res = true;
  ForEach<Lhs::cols()>([&]<auto col>() {
    ForEach<Lhs::rows()>([&]<auto row>() {
      if (!res)
        return;
      if (!(lhs(C<row>{}, C<col>{}) == rhs(C<row>{}, C<col>{})))
        res = false;
    });
  });

  return res;
}

//! The type ranges of `Lhs` and `Rhs` are intentionally unconstrained here,
//! because every `operator!=` must be generated directly from `operator==`.
template <typename Lhs, typename Rhs>
[[nodiscard]] ART_SPECIFIER constexpr bool operator!=(Lhs const &lhs, Rhs const &rhs) {
  return !(lhs == rhs);
}

template <is::Mat Lhs, is::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto operator+(Lhs const &lhs, Rhs const &rhs) {
  static_assert(
      Lhs::rows() == Rhs::rows() && Lhs::cols() == Rhs::cols(),
      "Binary operators are only defined for matrices with the same dimensions"
  );
  return detail::mat::OpBinary<[](auto const &l, auto const &r) { return l + r; }, Lhs::rows(), Lhs::cols()>(lhs, rhs);
}

template <is::Mat Lhs, is::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto operator-(Lhs const &lhs, Rhs const &rhs) {
  static_assert(
      Lhs::rows() == Rhs::rows() && Lhs::cols() == Rhs::cols(),
      "Binary operators are only defined for matrices with the same dimensions"
  );
  return detail::mat::OpBinary<[](auto const &l, auto const &r) { return l - r; }, Lhs::rows(), Lhs::cols()>(lhs, rhs);
}

template <is::Mat Lhs, isnot::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto operator*(Lhs const &lhs, Rhs const &rhs) {
  return detail::mat::OpBinary<[](auto const &l, auto const &r) { return l * r; }, Lhs::rows(), Lhs::cols()>(lhs, rhs);
}

template <isnot::Mat Lhs, is::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto operator*(Lhs const &lhs, Rhs const &rhs) {
  return detail::mat::OpBinary<[](auto const &l, auto const &r) { return l * r; }, Rhs::rows(), Rhs::cols()>(lhs, rhs);
}

template <is::Mat Lhs, isnot::Mat Rhs>
[[nodiscard]] ART_SPECIFIER constexpr auto operator/(Lhs const &lhs, Rhs const &rhs) {
  return detail::mat::OpBinary<[](auto const &l, auto const &r) { return l / r; }, Lhs::rows(), Lhs::cols()>(lhs, rhs);
}

//! The type range of `Rhs` is also intentionally unconstrained here.
template <is::Mat Lhs, typename Rhs>
ART_SPECIFIER constexpr Lhs &operator+=(Lhs &lhs, Rhs const &rhs) {
  lhs = lhs + rhs;
  return lhs;
}

template <is::Mat Lhs, typename Rhs>
ART_SPECIFIER constexpr Lhs &operator-=(Lhs &lhs, Rhs const &rhs) {
  lhs = lhs - rhs;
  return lhs;
}

template <is::Mat Lhs, typename Rhs>
ART_SPECIFIER constexpr Lhs &operator*=(Lhs &lhs, Rhs const &rhs) {
  lhs = lhs * rhs;
  return lhs;
}

template <is::Mat Lhs, typename Rhs>
ART_SPECIFIER constexpr Lhs &operator/=(Lhs &lhs, Rhs const &rhs) {
  lhs = lhs / rhs;
  return lhs;
}

//
//
//
//
//
// Aliases.

template <typename T, typename... Ts>
using Mat1 = Mat<T, 1, 1, Ts...>;
template <typename T, typename... Ts>
using Mat2 = Mat<T, 2, 2, Ts...>;
template <typename T, typename... Ts>
using Mat3 = Mat<T, 3, 3, Ts...>;
template <typename T, typename... Ts>
using Mat4 = Mat<T, 4, 4, Ts...>;

template <typename T, typename... Ts>
using Vec1 = Vec<T, 1, Ts...>;
template <typename T, typename... Ts>
using Vec2 = Vec<T, 2, Ts...>;
template <typename T, typename... Ts>
using Vec3 = Vec<T, 3, Ts...>;
template <typename T, typename... Ts>
using Vec4 = Vec<T, 4, Ts...>;

} // namespace arietta
