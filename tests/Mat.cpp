#include "Arietta/Mat.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

using Token = C<arietta::detail::mat::Deduce<int>::token>;

template <typename T>
struct AsConstants {
  using Constants = T;
};

struct A {};

[[nodiscard]] constexpr bool operator==(A const &, A const &) { return true; }

template <typename... Ts>
using Vec1f = Mat<float, 1, 1, Ts...>;
template <typename... Ts>
using Vec2f = Mat<float, 2, 1, Ts...>;
template <typename... Ts>
using Vec3f = Mat<float, 3, 1, Ts...>;
template <typename... Ts>
using Vec4f = Mat<float, 4, 1, Ts...>;

//
//
//
//
//
suite<"Mat"> _ = [] {
  "Detail"_test = [] {
    using namespace arietta::detail::mat;

    // Token.
    static_assert(is::Empty<Token>);

    // Concat constants row-wise.
    {
      using T0 = AsConstants<Types<Types<C<0>>>>;
      using T1 = AsConstants<Types<Types<C<1>>>>;
      using T2 = AsConstants<Types<Types<C<2>>>>;
      static_assert(is::Same<ConcatConstants<Row, T0>::type, Types<Types<C<0>>>>);
      static_assert(is::Same<ConcatConstants<Row, T0, T1>::type, Types<Types<C<0>, C<1>>>>);
      static_assert(is::Same<ConcatConstants<Row, T0, T1, T2>::type, Types<Types<C<0>, C<1>, C<2>>>>);
    }

    // Concat constants col-wise.
    {
      using T0 = AsConstants<Types<Types<C<0>>>>;
      using T1 = AsConstants<Types<Types<C<1>>>>;
      using T2 = AsConstants<Types<Types<C<2>>>>;
      static_assert(is::Same<ConcatConstants<Col, T0>::type, Types<Types<C<0>>>>);
      static_assert(is::Same<ConcatConstants<Col, T0, T1>::type, Types<Types<C<0>>, Types<C<1>>>>);
      static_assert(is::Same<ConcatConstants<Col, T0, T1, T2>::type, Types<Types<C<0>>, Types<C<1>>, Types<C<2>>>>);

      using T01 = AsConstants<Types<Types<C<0>, C<1>>>>;
      using T23 = AsConstants<Types<Types<C<2>, C<3>>>>;
      using T45 = AsConstants<Types<Types<C<4>, C<5>>>>;
      static_assert(is::Same<
                    ConcatConstants<Col, T01>::type, //
                    Types<Types<C<0>, C<1>>>>);
      static_assert(is::Same<
                    ConcatConstants<Col, T01, T23>::type, //
                    Types<Types<C<0>, C<1>>, Types<C<2>, C<3>>>>);
      static_assert(is::Same<
                    ConcatConstants<Col, T01, T23, T45>::type, //
                    Types<Types<C<0>, C<1>>, Types<C<2>, C<3>>, Types<C<4>, C<5>>>>);

      using T012 = AsConstants<Types<Types<C<0>, C<1>, C<2>>>>;
      using T345 = AsConstants<Types<Types<C<3>, C<4>, C<5>>>>;
      using T678 = AsConstants<Types<Types<C<6>, C<7>, C<8>>>>;
      static_assert(is::Same<
                    ConcatConstants<Col, T012>::type, //
                    Types<Types<C<0>, C<1>, C<2>>>>);
      static_assert(is::Same<
                    ConcatConstants<Col, T012, T345>::type, //
                    Types<Types<C<0>, C<1>, C<2>>, Types<C<3>, C<4>, C<5>>>>);
      static_assert(is::Same<
                    ConcatConstants<Col, T012, T345, T678>::type, //
                    Types<Types<C<0>, C<1>, C<2>>, Types<C<3>, C<4>, C<5>>, Types<C<6>, C<7>, C<8>>>>);
    }
  };

  //
  //
  //
  "Deductions And CTAD"_test = [] {
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      // Mat m_{};           //! Should not compile.
      // constexpr Mat n_{}; //! Should not compile.

      Mat m_V{u0};
      Mat m_C{c0};
      constexpr Mat n_V{v0};
      constexpr Mat n_C{c0};
      static_assert(is::Same<decltype(m_V), Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C), Mat<T, 1, 1, Types<Types<C0>>, Token>>);
      static_assert(is::Same<decltype(n_V), Mat<T, 1, 1, Types<Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C), Mat<T, 1, 1, Types<Types<C0>>, Token> const>);

      Mat m_VV{u0, u1};
      Mat m_CV{c0, u1};
      Mat m_VC{u0, c1};
      Mat m_CC{c0, c1};
      constexpr Mat n_VV{v0, v1};
      constexpr Mat n_CV{c0, v1};
      constexpr Mat n_VC{v0, c1};
      constexpr Mat n_CC{c0, c1};
      static_assert(is::Same<decltype(m_VV), Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV), Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC), Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(is::Same<decltype(m_CC), Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);
      static_assert(is::Same<decltype(n_VV), Mat<T, 2, 1, Types<Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CV), Mat<T, 2, 1, Types<Types<C0, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VC), Mat<T, 2, 1, Types<Types<VO, C1>>, Token> const>);
      static_assert(is::Same<decltype(n_CC), Mat<T, 2, 1, Types<Types<C0, C1>>, Token> const>);

      Mat m_VVV{u0, u1, u2};
      Mat m_CVV{c0, u1, u2};
      Mat m_VCV{u0, c1, u2};
      Mat m_CCV{c0, c1, u2};
      Mat m_VVC{u0, u1, c2};
      Mat m_CVC{c0, u1, c2};
      Mat m_VCC{u0, c1, c2};
      Mat m_CCC{c0, c1, c2};
      constexpr Mat n_VVV{v0, v1, v2};
      constexpr Mat n_CVV{c0, v1, v2};
      constexpr Mat n_VCV{v0, c1, v2};
      constexpr Mat n_CCV{c0, c1, v2};
      constexpr Mat n_VVC{v0, v1, c2};
      constexpr Mat n_CVC{c0, v1, c2};
      constexpr Mat n_VCC{v0, c1, c2};
      constexpr Mat n_CCC{c0, c1, c2};
      static_assert(is::Same<decltype(m_VVV), Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CVV), Mat<T, 3, 1, Types<Types<C0, VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VCV), Mat<T, 3, 1, Types<Types<VO, C1, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CCV), Mat<T, 3, 1, Types<Types<C0, C1, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VVC), Mat<T, 3, 1, Types<Types<VO, VO, C2>>, Token>>);
      static_assert(is::Same<decltype(m_CVC), Mat<T, 3, 1, Types<Types<C0, VO, C2>>, Token>>);
      static_assert(is::Same<decltype(m_VCC), Mat<T, 3, 1, Types<Types<VO, C1, C2>>, Token>>);
      static_assert(is::Same<decltype(m_CCC), Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token>>);
      static_assert(is::Same<decltype(n_VVV), Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CVV), Mat<T, 3, 1, Types<Types<C0, VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VCV), Mat<T, 3, 1, Types<Types<VO, C1, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CCV), Mat<T, 3, 1, Types<Types<C0, C1, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VVC), Mat<T, 3, 1, Types<Types<VO, VO, C2>>, Token> const>);
      static_assert(is::Same<decltype(n_CVC), Mat<T, 3, 1, Types<Types<C0, VO, C2>>, Token> const>);
      static_assert(is::Same<decltype(n_VCC), Mat<T, 3, 1, Types<Types<VO, C1, C2>>, Token> const>);
      static_assert(is::Same<decltype(n_CCC), Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token> const>);

      // Compatibility with copy and move constructors.
      Mat m_V0{Mat{u0}};
      Mat m_C0{Mat{c0}};
      Mat m_V1{m_V0};
      Mat m_C1{m_C0};
      Mat m_V2{std::move(m_V0)};
      Mat m_C2{std::move(m_C0)};
      constexpr Mat n_V0{Mat{v0}};
      constexpr Mat n_C0{Mat{c0}};
      constexpr Mat n_V1{n_V0};
      constexpr Mat n_C1{n_C0};
      static_assert(is::Same<decltype(m_V0), Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C0), Mat<T, 1, 1, Types<Types<C0>>, Token>>);
      static_assert(is::Same<decltype(m_V1), Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C1), Mat<T, 1, 1, Types<Types<C0>>, Token>>);
      static_assert(is::Same<decltype(m_V2), Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C2), Mat<T, 1, 1, Types<Types<C0>>, Token>>);
      static_assert(is::Same<decltype(n_V0), Mat<T, 1, 1, Types<Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C0), Mat<T, 1, 1, Types<Types<C0>>, Token> const>);
      static_assert(is::Same<decltype(n_V1), Mat<T, 1, 1, Types<Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C1), Mat<T, 1, 1, Types<Types<C0>>, Token> const>);

      Mat m_V_V{Mat{u0}, Mat{u1}};
      Mat m_C_V{Mat{c0}, Mat{u1}};
      Mat m_V_C{Mat{u0}, Mat{c1}};
      Mat m_C_C{Mat{c0}, Mat{c1}};
      constexpr Mat n_V_V{Mat{v0}, Mat{v1}};
      constexpr Mat n_C_V{Mat{c0}, Mat{v1}};
      constexpr Mat n_V_C{Mat{v0}, Mat{c1}};
      constexpr Mat n_C_C{Mat{c0}, Mat{c1}};
      static_assert(is::Same<decltype(m_V_V), Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C_V), Mat<T, 1, 2, Types<Types<C0>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_V_C), Mat<T, 1, 2, Types<Types<VO>, Types<C1>>, Token>>);
      static_assert(is::Same<decltype(m_C_C), Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token>>);
      static_assert(is::Same<decltype(n_V_V), Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C_V), Mat<T, 1, 2, Types<Types<C0>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_V_C), Mat<T, 1, 2, Types<Types<VO>, Types<C1>>, Token> const>);
      static_assert(is::Same<decltype(n_C_C), Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token> const>);

      Mat m_V_V_V{Mat{u0}, Mat{u1}, Mat{u2}};
      Mat m_C_V_V{Mat{c0}, Mat{u1}, Mat{u2}};
      Mat m_V_C_V{Mat{u0}, Mat{c1}, Mat{u2}};
      Mat m_C_C_V{Mat{c0}, Mat{c1}, Mat{u2}};
      Mat m_V_V_C{Mat{u0}, Mat{u1}, Mat{c2}};
      Mat m_C_V_C{Mat{c0}, Mat{u1}, Mat{c2}};
      Mat m_V_C_C{Mat{u0}, Mat{c1}, Mat{c2}};
      Mat m_C_C_C{Mat{c0}, Mat{c1}, Mat{c2}};
      constexpr Mat n_V_V_V{Mat{v0}, Mat{v1}, Mat{v2}};
      constexpr Mat n_C_V_V{Mat{c0}, Mat{v1}, Mat{v2}};
      constexpr Mat n_V_C_V{Mat{v0}, Mat{c1}, Mat{v2}};
      constexpr Mat n_C_C_V{Mat{c0}, Mat{c1}, Mat{v2}};
      constexpr Mat n_V_V_C{Mat{v0}, Mat{v1}, Mat{c2}};
      constexpr Mat n_C_V_C{Mat{c0}, Mat{v1}, Mat{c2}};
      constexpr Mat n_V_C_C{Mat{v0}, Mat{c1}, Mat{c2}};
      constexpr Mat n_C_C_C{Mat{c0}, Mat{c1}, Mat{c2}};
      static_assert(is::Same<decltype(m_V_V_V), Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C_V_V), Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_V_C_V), Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_C_C_V), Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<VO>>, Token>>);
      static_assert(is::Same<decltype(m_V_V_C), Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<C2>>, Token>>);
      static_assert(is::Same<decltype(m_C_V_C), Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<C2>>, Token>>);
      static_assert(is::Same<decltype(m_V_C_C), Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<C2>>, Token>>);
      static_assert(is::Same<decltype(m_C_C_C), Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token>>);
      static_assert(is::Same<decltype(n_V_V_V), Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C_V_V), Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_V_C_V), Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_C_C_V), Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<VO>>, Token> const>);
      static_assert(is::Same<decltype(n_V_V_C), Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<C2>>, Token> const>);
      static_assert(is::Same<decltype(n_C_V_C), Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<C2>>, Token> const>);
      static_assert(is::Same<decltype(n_V_C_C), Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<C2>>, Token> const>);
      static_assert(is::Same<decltype(n_C_C_C), Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token> const>);

      // Compatibility with copy and move constructors.
      Mat m_VV0{Mat{u0, u1}};
      Mat m_CV0{Mat{c0, u1}};
      Mat m_VC0{Mat{u0, c1}};
      Mat m_CC0{Mat{c0, c1}};
      Mat m_VV1{m_VV0};
      Mat m_CV1{m_CV0};
      Mat m_VC1{m_VC0};
      Mat m_CC1{m_CC0};
      Mat m_VV2{std::move(m_VV0)};
      Mat m_CV2{std::move(m_CV0)};
      Mat m_VC2{std::move(m_VC0)};
      Mat m_CC2{std::move(m_CC0)};
      constexpr Mat n_VV0{Mat{v0, v1}};
      constexpr Mat n_CV0{Mat{c0, v1}};
      constexpr Mat n_VC0{Mat{v0, c1}};
      constexpr Mat n_CC0{Mat{c0, c1}};
      constexpr Mat n_VV1{n_VV0};
      constexpr Mat n_CV1{n_CV0};
      constexpr Mat n_VC1{n_VC0};
      constexpr Mat n_CC1{n_CC0};
      static_assert(is::Same<decltype(m_VV0), Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV0), Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC0), Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(is::Same<decltype(m_CC0), Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);
      static_assert(is::Same<decltype(m_VV1), Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV1), Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC1), Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(is::Same<decltype(m_CC1), Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);
      static_assert(is::Same<decltype(m_VV2), Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV2), Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC2), Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(is::Same<decltype(m_CC2), Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);
      static_assert(is::Same<decltype(n_VV0), Mat<T, 2, 1, Types<Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CV0), Mat<T, 2, 1, Types<Types<C0, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VC0), Mat<T, 2, 1, Types<Types<VO, C1>>, Token> const>);
      static_assert(is::Same<decltype(n_CC0), Mat<T, 2, 1, Types<Types<C0, C1>>, Token> const>);
      static_assert(is::Same<decltype(n_VV1), Mat<T, 2, 1, Types<Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CV1), Mat<T, 2, 1, Types<Types<C0, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VC1), Mat<T, 2, 1, Types<Types<VO, C1>>, Token> const>);
      static_assert(is::Same<decltype(n_CC1), Mat<T, 2, 1, Types<Types<C0, C1>>, Token> const>);

      Mat m_VV_VV{Mat{u0, u1}, Mat{u2, u3}};
      Mat m_CV_VV{Mat{c0, u1}, Mat{u2, u3}};
      Mat m_VC_VV{Mat{u0, c1}, Mat{u2, u3}};
      Mat m_CC_VV{Mat{c0, c1}, Mat{u2, u3}};
      Mat m_VV_CV{Mat{u0, u1}, Mat{c2, u3}};
      Mat m_CV_CV{Mat{c0, u1}, Mat{c2, u3}};
      Mat m_VC_CV{Mat{u0, c1}, Mat{c2, u3}};
      Mat m_CC_CV{Mat{c0, c1}, Mat{c2, u3}};
      Mat m_VV_VC{Mat{u0, u1}, Mat{u2, c3}};
      Mat m_CV_VC{Mat{c0, u1}, Mat{u2, c3}};
      Mat m_VC_VC{Mat{u0, c1}, Mat{u2, c3}};
      Mat m_CC_VC{Mat{c0, c1}, Mat{u2, c3}};
      Mat m_VV_CC{Mat{u0, u1}, Mat{c2, c3}};
      Mat m_CV_CC{Mat{c0, u1}, Mat{c2, c3}};
      Mat m_VC_CC{Mat{u0, c1}, Mat{c2, c3}};
      Mat m_CC_CC{Mat{c0, c1}, Mat{c2, c3}};
      constexpr Mat n_VV_VV{Mat{v0, v1}, Mat{v2, v3}};
      constexpr Mat n_CV_VV{Mat{c0, v1}, Mat{v2, v3}};
      constexpr Mat n_VC_VV{Mat{v0, c1}, Mat{v2, v3}};
      constexpr Mat n_CC_VV{Mat{c0, c1}, Mat{v2, v3}};
      constexpr Mat n_VV_CV{Mat{v0, v1}, Mat{c2, v3}};
      constexpr Mat n_CV_CV{Mat{c0, v1}, Mat{c2, v3}};
      constexpr Mat n_VC_CV{Mat{v0, c1}, Mat{c2, v3}};
      constexpr Mat n_CC_CV{Mat{c0, c1}, Mat{c2, v3}};
      constexpr Mat n_VV_VC{Mat{v0, v1}, Mat{v2, c3}};
      constexpr Mat n_CV_VC{Mat{c0, v1}, Mat{v2, c3}};
      constexpr Mat n_VC_VC{Mat{v0, c1}, Mat{v2, c3}};
      constexpr Mat n_CC_VC{Mat{c0, c1}, Mat{v2, c3}};
      constexpr Mat n_VV_CC{Mat{v0, v1}, Mat{c2, c3}};
      constexpr Mat n_CV_CC{Mat{c0, v1}, Mat{c2, c3}};
      constexpr Mat n_VC_CC{Mat{v0, c1}, Mat{c2, c3}};
      constexpr Mat n_CC_CC{Mat{c0, c1}, Mat{c2, c3}};
      static_assert(is::Same<decltype(m_VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV_VV), Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC_VV), Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CC_VV), Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CV_CV), Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VC_CV), Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, VO>>, Token>>);
      static_assert(is::Same<decltype(m_CC_CV), Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, VO>>, Token>>);
      static_assert(is::Same<decltype(m_VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C3>>, Token>>);
      static_assert(is::Same<decltype(m_CV_VC), Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, C3>>, Token>>);
      static_assert(is::Same<decltype(m_VC_VC), Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, C3>>, Token>>);
      static_assert(is::Same<decltype(m_CC_VC), Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, C3>>, Token>>);
      static_assert(is::Same<decltype(m_VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, C3>>, Token>>);
      static_assert(is::Same<decltype(m_CV_CC), Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, C3>>, Token>>);
      static_assert(is::Same<decltype(m_VC_CC), Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, C3>>, Token>>);
      static_assert(is::Same<decltype(m_CC_CC), Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token>>);
      static_assert(is::Same<decltype(n_VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CV_VV), Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VC_VV), Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CC_VV), Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CV_CV), Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VC_CV), Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_CC_CV), Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, VO>>, Token> const>);
      static_assert(is::Same<decltype(n_VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_CV_VC), Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_VC_VC), Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_CC_VC), Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_CV_CC), Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_VC_CC), Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, C3>>, Token> const>);
      static_assert(is::Same<decltype(n_CC_CC), Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token> const>);
    });

    ForEach<Types<std::array<f32, 8>, A>>([]<typename T> {
      Mat m_V{T{}};
      Mat m_C{C<T{}>{}};
      constexpr Mat n_V{T{}};
      constexpr Mat n_C{C<T{}>{}};
      static_assert(is::Same<decltype(m_V), Mat<T, 1, 1, Types<Types<void>>, Token>>);
      static_assert(is::Same<decltype(m_C), Mat<T, 1, 1, Types<Types<C<T{}>>>, Token>>);
      static_assert(is::Same<decltype(n_V), Mat<T, 1, 1, Types<Types<void>>, Token> const>);
      static_assert(is::Same<decltype(n_C), Mat<T, 1, 1, Types<Types<C<T{}>>>, Token> const>);
    });

    ForEach<Types<std::string, std::vector<f32>>>([]<typename T> {
      Mat m_V{T{}};
      static_assert(is::Same<decltype(m_V), Mat<T, 1, 1, Types<Types<void>>, Token>>);
    });
  };

  //
  //
  //
  "Size Of"_test = [] {
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      static_assert(sizeof(Mat<T, 1, 1, Types<Types<VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 1, Types<Types<C0>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 1, 1, Types<Types<C0>>, Token>>);

      static_assert(sizeof(Mat<T, 2, 1, Types<Types<VO, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 1, Types<Types<C0, VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 1, Types<Types<VO, C1>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 1, Types<Types<C0, C1>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);

      static_assert(sizeof(Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<C0, VO, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<VO, C1, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<C0, C1, VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<VO, VO, C2>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<C0, VO, C2>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<VO, C1, C2>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token>>);

      static_assert(sizeof(Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 2, Types<Types<C0>, Types<VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 2, Types<Types<VO>, Types<C1>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token>>);

      static_assert(sizeof(Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<C2>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<C2>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<C2>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token>>);

      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>) == 4 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, VO>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, VO>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, VO>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, VO>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, VO>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C3>>, Token>) == 3 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, C3>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, C3>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, C3>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, C3>>, Token>) == 2 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, C3>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, C3>>, Token>) == 1 * sizeof(T));
      static_assert(sizeof(Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token>) == 1);
      static_assert(is::Empty<Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token>>);
    });
  };

  //
  //
  //
  "Constructors"_test = [] {
    // Default constructors.
    // TODO: Testing all of these type combinations significantly increases compilation time
    //       and may exceed the maximum allowed number of sections.
    ForEach<Types</*i8, u8, i16, u16,*/ i32, u32, i64, u64, /*isize, usize,*/ f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        Mat<T, 1, 1, Types<Types<Cs0>>, Token> m{};
        constexpr Mat<T, 1, 1, Types<Types<Cs0>>, Token> n{};

        // Runtime indices.
        if constexpr (is::Same<Cs0, VO>) {
          expect(m[0, 0] == 0);
          expect(m[0] == 0);
          static_assert(n[0, 0] == 0);
          static_assert(n[0] == 0);
        } else {
          // expect(m[0, 0] == 0); //! Should not compile.
          // static_assert(n[0, 0] == 0); //! Should not compile.
        }

        // Compile-time indices.
        auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
          if constexpr (is::Same<Csi, VO>) {
            expect(m[x, y] == 0);
            static_assert(n[x, y] == 0);
            static_assert(is::Same<decltype(m[x, y]), T &>);
            static_assert(is::Same<decltype(n[x, y]), T const &>);
          } else {
            static_assert(m[x, y] == Ci{});
            static_assert(n[x, y] == Ci{});
            static_assert(is::Same<decltype(m[x, y]), Ci>);
            static_assert(is::Same<decltype(n[x, y]), Ci>);
          }
        };
        testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> m{};
          constexpr Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> n{};

          // Runtime indices.
          if constexpr (is::Same<Cs0, VO> && is::Same<Cs1, VO>) {
            expect(m[0, 0] == 0);
            expect(m[1, 0] == 0);
            expect(m[0] == 0);
            expect(m[1] == 0);
            static_assert(n[0, 0] == 0);
            static_assert(n[1, 0] == 0);
            static_assert(n[0] == 0);
            static_assert(n[1] == 0);
          } else {
            // expect(m[0, 0] == 0); //! Should not compile.
            // static_assert(n[0, 0] == 0); //! Should not compile.
          }

          // Compile-time indices.
          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>) {
              expect(m[x, y] == 0);
              static_assert(n[x, y] == 0);
              static_assert(is::Same<decltype(m[x, y]), T &>);
              static_assert(is::Same<decltype(n[x, y]), T const &>);
            } else {
              static_assert(m[x, y] == Ci{});
              static_assert(n[x, y] == Ci{});
              static_assert(is::Same<decltype(m[x, y]), Ci>);
              static_assert(is::Same<decltype(n[x, y]), Ci>);
            }
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
        });
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> m{};
            constexpr Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> n{};

            // Runtime indices.
            if constexpr (is::Same<Cs0, VO> && is::Same<Cs1, VO> && is::Same<Cs2, VO>) {
              expect(m[0, 0] == 0);
              expect(m[1, 0] == 0);
              expect(m[2, 0] == 0);
              expect(m[0] == 0);
              expect(m[1] == 0);
              expect(m[2] == 0);
              static_assert(n[0, 0] == 0);
              static_assert(n[1, 0] == 0);
              static_assert(n[2, 0] == 0);
              static_assert(n[0] == 0);
              static_assert(n[1] == 0);
              static_assert(n[2] == 0);
            } else {
              // expect(m[0, 0] == 0); //! Should not compile.
              // static_assert(n[0, 0] == 0); //! Should not compile.
            }

            // Compile-time indices.
            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>) {
                expect(m[x, y] == 0);
                static_assert(n[x, y] == 0);
                static_assert(is::Same<decltype(m[x, y]), T &>);
                static_assert(is::Same<decltype(n[x, y]), T const &>);
              } else {
                static_assert(m[x, y] == Ci{});
                static_assert(n[x, y] == Ci{});
                static_assert(is::Same<decltype(m[x, y]), Ci>);
                static_assert(is::Same<decltype(n[x, y]), Ci>);
              }
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
            testIndices.template operator()<Cs2, I2{}, I0{}, C2>();
          });
        });
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 1, 2, Types<Types<Cs0>, Types<Cs1>>, Token> m{};
          constexpr Mat<T, 1, 2, Types<Types<Cs0>, Types<Cs1>>, Token> n{};

          // Runtime indices.
          if constexpr (is::Same<Cs0, VO> && is::Same<Cs1, VO>) {
            expect(m[0, 0] == 0);
            expect(m[0, 1] == 0);
            static_assert(n[0, 0] == 0);
            static_assert(n[0, 1] == 0);
          } else {
            // expect(m[0, 0] == 0); //! Should not compile.
            // static_assert(n[0, 0] == 0); //! Should not compile.
          }

          // Compile-time indices.
          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>) {
              expect(m[x, y] == 0);
              static_assert(n[x, y] == 0);
              static_assert(is::Same<decltype(m[x, y]), T &>);
              static_assert(is::Same<decltype(n[x, y]), T const &>);
            } else {
              static_assert(m[x, y] == Ci{});
              static_assert(n[x, y] == Ci{});
              static_assert(is::Same<decltype(m[x, y]), Ci>);
              static_assert(is::Same<decltype(n[x, y]), Ci>);
            }
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
        });
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 1, 3, Types<Types<Cs0>, Types<Cs1>, Types<Cs2>>, Token> m{};
            constexpr Mat<T, 1, 3, Types<Types<Cs0>, Types<Cs1>, Types<Cs2>>, Token> n{};

            // Runtime indices.
            if constexpr (is::Same<Cs0, VO> && is::Same<Cs1, VO> && is::Same<Cs2, VO>) {
              expect(m[0, 0] == 0);
              expect(m[0, 1] == 0);
              expect(m[0, 2] == 0);
              static_assert(n[0, 0] == 0);
              static_assert(n[0, 1] == 0);
              static_assert(n[0, 2] == 0);
            } else {
              // expect(m[0, 0] == 0); //! Should not compile.
              // static_assert(n[0, 0] == 0); //! Should not compile.
            }

            // Compile-time indices.
            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>) {
                expect(m[x, y] == 0);
                static_assert(n[x, y] == 0);
                static_assert(is::Same<decltype(m[x, y]), T &>);
                static_assert(is::Same<decltype(n[x, y]), T const &>);
              } else {
                static_assert(m[x, y] == Ci{});
                static_assert(n[x, y] == Ci{});
                static_assert(is::Same<decltype(m[x, y]), Ci>);
                static_assert(is::Same<decltype(n[x, y]), Ci>);
              }
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
            testIndices.template operator()<Cs2, I0{}, I2{}, C2>();
          });
        });
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            ForEach<Types<VO, C3>>([&]<typename Cs3> {
              Mat<T, 2, 2, Types<Types<Cs0, Cs1>, Types<Cs2, Cs3>>, Token> m{};
              constexpr Mat<T, 2, 2, Types<Types<Cs0, Cs1>, Types<Cs2, Cs3>>, Token> n{};

              // Runtime indices.
              if constexpr (is::Same<Cs0, VO> && is::Same<Cs1, VO> && is::Same<Cs2, VO> && is::Same<Cs3, VO>) {
                expect(m[0, 0] == 0);
                expect(m[1, 0] == 0);
                expect(m[0, 1] == 0);
                expect(m[1, 1] == 0);
                static_assert(n[0, 0] == 0);
                static_assert(n[1, 0] == 0);
                static_assert(n[0, 1] == 0);
                static_assert(n[1, 1] == 0);
              } else {
                // expect(m[0, 0] == 0); //! Should not compile.
                // static_assert(n[0, 0] == 0); //! Should not compile.
              }

              // Compile-time indices.
              auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>) {
                  expect(m[x, y] == 0);
                  static_assert(n[x, y] == 0);
                  static_assert(is::Same<decltype(m[x, y]), T &>);
                  static_assert(is::Same<decltype(n[x, y]), T const &>);
                } else {
                  static_assert(m[x, y] == Ci{});
                  static_assert(n[x, y] == Ci{});
                  static_assert(is::Same<decltype(m[x, y]), Ci>);
                  static_assert(is::Same<decltype(n[x, y]), Ci>);
                }
              };
              testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
              testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
              testIndices.template operator()<Cs2, I0{}, I1{}, C2>();
              testIndices.template operator()<Cs3, I1{}, I1{}, C3>();
            });
          });
        });
      });
    });

    // Deduction-based constructors.
    ForEach<Types</*i8, u8, i16, u16, i32, u32, i64, u64, isize, usize,*/ f32 /*, f64*/>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;

        auto test = [&](auto... args) {
          Mat<T, 1, 1, Types<Types<Cs0>>, Token> m{args...};

          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x, y] == Ci{});
            else
              static_assert(m[x, y] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
        };
        test(Arg0{C0{}});
        test(C0{});
        test(Mat{Arg0{C0{}}});
        test(Mat{C0{}});
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;
          using Arg1 = std::conditional_t<is::Same<Cs1, VO>, T, Cs1>;

          auto test = [&](auto... args) {
            Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> m{args...};

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{});
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
          };
          test(Arg0{C0{}}, Arg1{C1{}});
          test(C0{}, C1{});
          test(Mat{Arg0{C0{}}, Arg1{C1{}}});
          test(Mat{C0{}, C1{}});
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;
            using Arg1 = std::conditional_t<is::Same<Cs1, VO>, T, Cs1>;
            using Arg2 = std::conditional_t<is::Same<Cs2, VO>, T, Cs2>;

            auto test = [&](auto... args) {
              Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> m{args...};

              auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>)
                  expect(m[x, y] == Ci{});
                else
                  static_assert(m[x, y] == Ci{});
              };
              testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
              testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
              testIndices.template operator()<Cs2, I2{}, I0{}, C2>();
            };
            test(Arg0{C0{}}, Arg1{C1{}}, Arg2{C2{}});
            test(C0{}, C1{}, C2{});
            test(Mat{Arg0{C0{}}, Arg1{C1{}}, Arg2{C2{}}});
            test(Mat{C0{}, C1{}, C2{}});
          });
        });
      });
#endif

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;
          using Arg1 = std::conditional_t<is::Same<Cs1, VO>, T, Cs1>;

          auto test = [&](auto... args) {
            Mat<T, 1, 2, Types<Types<Cs0>, Types<Cs1>>, Token> m{args...};

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{});
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
          };
          test(Mat{Arg0{C0{}}}, Mat{Arg1{C1{}}});
          test(Mat{C0{}}, Mat{C1{}});
          test(Mat{Mat{Arg0{C0{}}}, Mat{Arg1{C1{}}}});
          test(Mat{Mat{C0{}}, Mat{C1{}}});
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;
            using Arg1 = std::conditional_t<is::Same<Cs1, VO>, T, Cs1>;
            using Arg2 = std::conditional_t<is::Same<Cs2, VO>, T, Cs2>;

            auto test = [&](auto... args) {
              Mat<T, 1, 3, Types<Types<Cs0>, Types<Cs1>, Types<Cs2>>, Token> m{args...};

              auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>)
                  expect(m[x, y] == Ci{});
                else
                  static_assert(m[x, y] == Ci{});
              };
              testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
              testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
              testIndices.template operator()<Cs2, I0{}, I2{}, C2>();
            };
            test(Mat{Arg0{C0{}}}, Mat{Arg1{C1{}}}, Mat{Arg2{C2{}}});
            test(Mat{C0{}}, Mat{C1{}}, Mat{C2{}});
            test(Mat{Mat{Arg0{C0{}}}, Mat{Arg1{C1{}}}, Mat{Arg2{C2{}}}});
            test(Mat{Mat{C0{}}, Mat{C1{}}, Mat{C2{}}});
          });
        });
      });
#endif

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            ForEach<Types<VO, C3>>([&]<typename Cs3> {
              using Arg0 = std::conditional_t<is::Same<Cs0, VO>, T, Cs0>;
              using Arg1 = std::conditional_t<is::Same<Cs1, VO>, T, Cs1>;
              using Arg2 = std::conditional_t<is::Same<Cs2, VO>, T, Cs2>;
              using Arg3 = std::conditional_t<is::Same<Cs3, VO>, T, Cs3>;

              auto test = [&](auto... args) {
                Mat<T, 2, 2, Types<Types<Cs0, Cs1>, Types<Cs2, Cs3>>, Token> m{args...};

                auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                  if constexpr (is::Same<Csi, VO>)
                    expect(m[x, y] == Ci{});
                  else
                    static_assert(m[x, y] == Ci{});
                };
                testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
                testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
                testIndices.template operator()<Cs2, I0{}, I1{}, C2>();
                testIndices.template operator()<Cs3, I1{}, I1{}, C3>();
              };
              test(Mat{Arg0{C0{}}, Arg1{C1{}}}, Mat{Arg2{C2{}}, Arg3{C3{}}});
              test(Mat{C0{}, C1{}}, Mat{C2{}, C3{}});
              test(Mat{Mat{Arg0{C0{}}, Arg1{C1{}}}, Mat{Arg2{C2{}}, Arg3{C3{}}}});
              test(Mat{Mat{C0{}, C1{}}, Mat{C2{}, C3{}}});
            });
          });
        });
      });
#endif
    });
  };

  //
  //
  //
  "Assignments"_test = [] {
    // Assignment operators.
    ForEach<Types</*i8, u8, i16, u16, i32, u32, i64, u64, isize, usize,*/ f32 /*, f64*/>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        Mat<T, 1, 1, Types<Types<Cs0>>, Token> m{};
        Mat m1{C0{}};
        m = m1;

        auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
          if constexpr (is::Same<Csi, VO>)
            expect(m[x, y] == Ci{});
          else
            static_assert(m[x, y] == Ci{});
        };
        testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> m{};
          Mat m1{C0{}, C1{}};
          m = m1;

          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x, y] == Ci{});
            else
              static_assert(m[x, y] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> m{};
            Mat m1{C0{}, C1{}, C2{}};
            m = m1;

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{});
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
            testIndices.template operator()<Cs2, I2{}, I0{}, C2>();
          });
        });
      });
#endif

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 1, 2, Types<Types<Cs0>, Types<Cs1>>, Token> m{};
          Mat m1{Mat{C0{}}, Mat{C1{}}};
          m = m1;

          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x, y] == Ci{});
            else
              static_assert(m[x, y] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 1, 3, Types<Types<Cs0>, Types<Cs1>, Types<Cs2>>, Token> m{};
            Mat m1{Mat{C0{}}, Mat{C1{}}, Mat{C2{}}};
            m = m1;

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{});
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
            testIndices.template operator()<Cs2, I0{}, I2{}, C2>();
          });
        });
      });
#endif

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            ForEach<Types<VO, C3>>([&]<typename Cs3> {
              Mat<T, 2, 2, Types<Types<Cs0, Cs1>, Types<Cs2, Cs3>>, Token> m{};
              Mat m1{Mat{C0{}, C1{}}, Mat{C2{}, C3{}}};
              m = m1;

              auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>)
                  expect(m[x, y] == Ci{});
                else
                  static_assert(m[x, y] == Ci{});
              };
              testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
              testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
              testIndices.template operator()<Cs2, I0{}, I1{}, C2>();
              testIndices.template operator()<Cs3, I1{}, I1{}, C3>();
            });
          });
        });
      });
#endif
    });

    // `operator[]`.
    ForEach<Types</*i8, u8, i16, u16, i32, u32, i64, u64, isize, usize,*/ f32 /*, f64*/>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        Mat<T, 1, 1, Types<Types<Cs0>>, Token> m{};

        auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
          if constexpr (is::Same<Csi, VO>)
            m[x, y] = Ci{} + static_cast<T>(10);
          else
            m[x, y] = Ci{};
        };
        assign.template operator()<Cs0, I0{}, I0{}, C0>();

        auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
          if constexpr (is::Same<Csi, VO>)
            expect(m[x, y] == Ci{} + static_cast<T>(10));
          else
            static_assert(m[x, y] == Ci{});
        };
        testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> m{};

          auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              m[x, y] = Ci{} + static_cast<T>(10);
            else
              m[x, y] = Ci{};
          };
          assign.template operator()<Cs0, I0{}, I0{}, C0>();
          assign.template operator()<Cs1, I1{}, I0{}, C1>();

          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x, y] == Ci{} + static_cast<T>(10));
            else
              static_assert(m[x, y] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> m{};

            auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                m[x, y] = Ci{} + static_cast<T>(10);
              else
                m[x, y] = Ci{};
            };
            assign.template operator()<Cs0, I0{}, I0{}, C0>();
            assign.template operator()<Cs1, I1{}, I0{}, C1>();
            assign.template operator()<Cs2, I2{}, I0{}, C2>();

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{} + static_cast<T>(10));
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
            testIndices.template operator()<Cs2, I2{}, I0{}, C2>();
          });
        });
      });
#endif

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 1, 2, Types<Types<Cs0>, Types<Cs1>>, Token> m{};

          auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              m[x, y] = Ci{} + static_cast<T>(10);
            else
              m[x, y] = Ci{};
          };
          assign.template operator()<Cs0, I0{}, I0{}, C0>();
          assign.template operator()<Cs1, I0{}, I1{}, C1>();

          auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x, y] == Ci{} + static_cast<T>(10));
            else
              static_assert(m[x, y] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
          testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 1, 3, Types<Types<Cs0>, Types<Cs1>, Types<Cs2>>, Token> m{};

            auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                m[x, y] = Ci{} + static_cast<T>(10);
              else
                m[x, y] = Ci{};
            };
            assign.template operator()<Cs0, I0{}, I0{}, C0>();
            assign.template operator()<Cs1, I0{}, I1{}, C1>();
            assign.template operator()<Cs2, I0{}, I2{}, C2>();

            auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x, y] == Ci{} + static_cast<T>(10));
              else
                static_assert(m[x, y] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
            testIndices.template operator()<Cs1, I0{}, I1{}, C1>();
            testIndices.template operator()<Cs2, I0{}, I2{}, C2>();
          });
        });
      });
#endif

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            ForEach<Types<VO, C3>>([&]<typename Cs3> {
              Mat<T, 2, 2, Types<Types<Cs0, Cs1>, Types<Cs2, Cs3>>, Token> m{};

              auto assign = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>)
                  m[x, y] = Ci{} + static_cast<T>(10);
                else
                  m[x, y] = Ci{};
              };
              assign.template operator()<Cs0, I0{}, I0{}, C0>();
              assign.template operator()<Cs1, I1{}, I0{}, C1>();
              assign.template operator()<Cs2, I0{}, I1{}, C2>();
              assign.template operator()<Cs3, I1{}, I1{}, C3>();

              auto testIndices = [&]<typename Csi, auto x, auto y, typename Ci> {
                if constexpr (is::Same<Csi, VO>)
                  expect(m[x, y] == Ci{} + static_cast<T>(10));
                else
                  static_assert(m[x, y] == Ci{});
              };
              testIndices.template operator()<Cs0, I0{}, I0{}, C0>();
              testIndices.template operator()<Cs1, I1{}, I0{}, C1>();
              testIndices.template operator()<Cs2, I0{}, I1{}, C2>();
              testIndices.template operator()<Cs3, I1{}, I1{}, C3>();
            });
          });
        });
      });
#endif
    });

    ForEach<Types</*i8, u8, i16, u16, i32, u32, i64, u64, isize, usize,*/ f32 /*, f64*/>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        Mat<T, 1, 1, Types<Types<Cs0>>, Token> m{};

        auto assign = [&]<typename Csi, auto x, typename Ci> {
          if constexpr (is::Same<Csi, VO>)
            m[x] = Ci{} + static_cast<T>(10);
          else
            m[x] = Ci{};
        };
        assign.template operator()<Cs0, I0{}, C0>();

        auto testIndices = [&]<typename Csi, auto x, typename Ci> {
          if constexpr (is::Same<Csi, VO>)
            expect(m[x] == Ci{} + static_cast<T>(10));
          else
            static_assert(m[x] == Ci{});
        };
        testIndices.template operator()<Cs0, I0{}, C0>();
      });

      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          Mat<T, 2, 1, Types<Types<Cs0, Cs1>>, Token> m{};

          auto assign = [&]<typename Csi, auto x, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              m[x] = Ci{} + static_cast<T>(10);
            else
              m[x] = Ci{};
          };
          assign.template operator()<Cs0, I0{}, C0>();
          assign.template operator()<Cs1, I1{}, C1>();

          auto testIndices = [&]<typename Csi, auto x, typename Ci> {
            if constexpr (is::Same<Csi, VO>)
              expect(m[x] == Ci{} + static_cast<T>(10));
            else
              static_assert(m[x] == Ci{});
          };
          testIndices.template operator()<Cs0, I0{}, C0>();
          testIndices.template operator()<Cs1, I1{}, C1>();
        });
      });

#if 0
      ForEach<Types<VO, C0>>([&]<typename Cs0> {
        ForEach<Types<VO, C1>>([&]<typename Cs1> {
          ForEach<Types<VO, C2>>([&]<typename Cs2> {
            Mat<T, 3, 1, Types<Types<Cs0, Cs1, Cs2>>, Token> m{};

            auto assign = [&]<typename Csi, auto x, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                m[x] = Ci{} + static_cast<T>(10);
              else
                m[x] = Ci{};
            };
            assign.template operator()<Cs0, I0{}, C0>();
            assign.template operator()<Cs1, I1{}, C1>();
            assign.template operator()<Cs2, I2{}, C2>();

            auto testIndices = [&]<typename Csi, auto x, typename Ci> {
              if constexpr (is::Same<Csi, VO>)
                expect(m[x] == Ci{} + static_cast<T>(10));
              else
                static_assert(m[x] == Ci{});
            };
            testIndices.template operator()<Cs0, I0{}, C0>();
            testIndices.template operator()<Cs1, I1{}, C1>();
            testIndices.template operator()<Cs2, I2{}, C2>();
          });
        });
      });
#endif
    });
  };

  //
  //
  //
  "Is And Is Not"_test = [] {
    // `is::Mat`.
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      static_assert(is::Mat<Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 1, Types<Types<C0>>, Token>>);

      static_assert(is::Mat<Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);

      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<C0, VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<VO, C1, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<C0, C1, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<VO, VO, C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<C0, VO, C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<VO, C1, C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token>>);

      static_assert(is::Mat<Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 2, Types<Types<C0>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 2, Types<Types<VO>, Types<C1>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token>>);

      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<C2>>, Token>>);
      static_assert(is::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token>>);

      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, VO>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, C3>>, Token>>);
      static_assert(is::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token>>);

      static_assert(!is::Mat<Mat<T, 1, 1>>);
      static_assert(!is::Mat<Mat<T, 2, 1>>);
      static_assert(!is::Mat<Mat<T, 3, 1>>);
      static_assert(!is::Mat<Mat<T, 1, 2>>);
      static_assert(!is::Mat<Mat<T, 1, 3>>);
      static_assert(!is::Mat<Mat<T, 2, 2>>);

      static_assert(!is::Mat<void>);
      static_assert(!is::Mat<C<0>>);
      static_assert(!is::Mat<std::array<T, 1>>);
    });

    // `isnot::Mat`.
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      static_assert(!isnot::Mat<Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 1, Types<Types<C0>>, Token>>);

      static_assert(!isnot::Mat<Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 1, Types<Types<C0, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 1, Types<Types<VO, C1>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 1, Types<Types<C0, C1>>, Token>>);

      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<C0, VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<VO, C1, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<C0, C1, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<VO, VO, C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<C0, VO, C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<VO, C1, C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 3, 1, Types<Types<C0, C1, C2>>, Token>>);

      static_assert(!isnot::Mat<Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 2, Types<Types<C0>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 2, Types<Types<VO>, Types<C1>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 2, Types<Types<C0>, Types<C1>>, Token>>);

      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<VO>, Types<C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<VO>, Types<C1>, Types<C2>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 1, 3, Types<Types<C0>, Types<C1>, Types<C2>>, Token>>);

      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, VO>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<VO, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<VO, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<VO, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, VO>, Types<C2, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, VO>, Types<C2, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<VO, C1>, Types<C2, C3>>, Token>>);
      static_assert(!isnot::Mat<Mat<T, 2, 2, Types<Types<C0, C1>, Types<C2, C3>>, Token>>);

      static_assert(isnot::Mat<Mat<T, 1, 1>>);
      static_assert(isnot::Mat<Mat<T, 2, 1>>);
      static_assert(isnot::Mat<Mat<T, 3, 1>>);
      static_assert(isnot::Mat<Mat<T, 1, 2>>);
      static_assert(isnot::Mat<Mat<T, 1, 3>>);
      static_assert(isnot::Mat<Mat<T, 2, 2>>);

      static_assert(isnot::Mat<void>);
      static_assert(isnot::Mat<C<0>>);
      static_assert(isnot::Mat<std::array<T, 1>>);
    });
  };

  //
  //
  //
  "As Storages"_test = [] {
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;

      typename Mat<T, 1, 1>::AsStorage s11{};
      typename Mat<T, 2, 1>::AsStorage s21{};
      typename Mat<T, 3, 1>::AsStorage s31{};
      typename Mat<T, 1, 2>::AsStorage s12{};
      typename Mat<T, 1, 3>::AsStorage s13{};
      typename Mat<T, 2, 2>::AsStorage s22{};
      static_assert(is::Same<typename Mat<T, 1, 1>::AsStorage, Mat<T, 1, 1, Types<Types<VO>>, Token>>);
      static_assert(is::Same<typename Mat<T, 2, 1>::AsStorage, Mat<T, 2, 1, Types<Types<VO, VO>>, Token>>);
      static_assert(is::Same<typename Mat<T, 3, 1>::AsStorage, Mat<T, 3, 1, Types<Types<VO, VO, VO>>, Token>>);
      static_assert(is::Same<typename Mat<T, 1, 2>::AsStorage, Mat<T, 1, 2, Types<Types<VO>, Types<VO>>, Token>>);
      static_assert(
          is::Same<typename Mat<T, 1, 3>::AsStorage, Mat<T, 1, 3, Types<Types<VO>, Types<VO>, Types<VO>>, Token>>
      );
      static_assert(
          is::Same<typename Mat<T, 2, 2>::AsStorage, Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>
      );
    });
  };

  //
  //
  //
  "Constants"_test = [] {
    // `Constant`.
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      auto m11_0 = Mat<T, 1, 1>::template Constant<v3>();
      auto m11_1 = Mat<T, 1, 1>::template Constant<C3>();
      auto m11_2 = Mat<T, 1, 1>::template Constant<c3>();
      constexpr auto n11_0 = Mat<T, 1, 1>::template Constant<v3>();
      constexpr auto n11_1 = Mat<T, 1, 1>::template Constant<C3>();
      constexpr auto n11_2 = Mat<T, 1, 1>::template Constant<c3>();
      static_assert(is::Same<decltype(m11_0), Mat<T, 1, 1, Types<Types<C3>>, Token>>);
      static_assert(is::Same<decltype(n11_0), Mat<T, 1, 1, Types<Types<C3>>, Token> const>);
      static_assert(is::Same<decltype(m11_1), decltype(m11_0)> && is::Same<decltype(m11_2), decltype(m11_0)>);
      static_assert(is::Same<decltype(n11_1), decltype(n11_0)> && is::Same<decltype(n11_2), decltype(n11_0)>);

      auto m21_0 = Mat<T, 2, 1>::template Constant<v3>();
      auto m21_1 = Mat<T, 2, 1>::template Constant<C3>();
      auto m21_2 = Mat<T, 2, 1>::template Constant<c3>();
      constexpr auto n21_0 = Mat<T, 2, 1>::template Constant<v3>();
      constexpr auto n21_1 = Mat<T, 2, 1>::template Constant<C3>();
      constexpr auto n21_2 = Mat<T, 2, 1>::template Constant<c3>();
      static_assert(is::Same<decltype(m21_0), Mat<T, 2, 1, Types<Types<C3, C3>>, Token>>);
      static_assert(is::Same<decltype(n21_0), Mat<T, 2, 1, Types<Types<C3, C3>>, Token> const>);
      static_assert(is::Same<decltype(m21_1), decltype(m21_0)> && is::Same<decltype(m21_2), decltype(m21_0)>);
      static_assert(is::Same<decltype(n21_1), decltype(n21_0)> && is::Same<decltype(n21_2), decltype(n21_0)>);

      auto m31_0 = Mat<T, 3, 1>::template Constant<v3>();
      auto m31_1 = Mat<T, 3, 1>::template Constant<C3>();
      auto m31_2 = Mat<T, 3, 1>::template Constant<c3>();
      constexpr auto n31_0 = Mat<T, 3, 1>::template Constant<v3>();
      constexpr auto n31_1 = Mat<T, 3, 1>::template Constant<C3>();
      constexpr auto n31_2 = Mat<T, 3, 1>::template Constant<c3>();
      static_assert(is::Same<decltype(m31_0), Mat<T, 3, 1, Types<Types<C3, C3, C3>>, Token>>);
      static_assert(is::Same<decltype(n31_0), Mat<T, 3, 1, Types<Types<C3, C3, C3>>, Token> const>);
      static_assert(is::Same<decltype(m31_1), decltype(m31_0)> && is::Same<decltype(m31_2), decltype(m31_0)>);
      static_assert(is::Same<decltype(n31_1), decltype(n31_0)> && is::Same<decltype(n31_2), decltype(n31_0)>);

      auto m12_0 = Mat<T, 1, 2>::template Constant<v3>();
      auto m12_1 = Mat<T, 1, 2>::template Constant<C3>();
      auto m12_2 = Mat<T, 1, 2>::template Constant<c3>();
      constexpr auto n12_0 = Mat<T, 1, 2>::template Constant<v3>();
      constexpr auto n12_1 = Mat<T, 1, 2>::template Constant<C3>();
      constexpr auto n12_2 = Mat<T, 1, 2>::template Constant<c3>();
      static_assert(is::Same<decltype(m12_0), Mat<T, 1, 2, Types<Types<C3>, Types<C3>>, Token>>);
      static_assert(is::Same<decltype(n12_0), Mat<T, 1, 2, Types<Types<C3>, Types<C3>>, Token> const>);
      static_assert(is::Same<decltype(m12_1), decltype(m12_0)> && is::Same<decltype(m12_2), decltype(m12_0)>);
      static_assert(is::Same<decltype(n12_1), decltype(n12_0)> && is::Same<decltype(n12_2), decltype(n12_0)>);

      auto m13_0 = Mat<T, 1, 3>::template Constant<v3>();
      auto m13_1 = Mat<T, 1, 3>::template Constant<C3>();
      auto m13_2 = Mat<T, 1, 3>::template Constant<c3>();
      constexpr auto n13_0 = Mat<T, 1, 3>::template Constant<v3>();
      constexpr auto n13_1 = Mat<T, 1, 3>::template Constant<C3>();
      constexpr auto n13_2 = Mat<T, 1, 3>::template Constant<c3>();
      static_assert(is::Same<decltype(m13_0), Mat<T, 1, 3, Types<Types<C3>, Types<C3>, Types<C3>>, Token>>);
      static_assert(is::Same<decltype(n13_0), Mat<T, 1, 3, Types<Types<C3>, Types<C3>, Types<C3>>, Token> const>);
      static_assert(is::Same<decltype(m13_1), decltype(m13_0)> && is::Same<decltype(m13_2), decltype(m13_0)>);
      static_assert(is::Same<decltype(n13_1), decltype(n13_0)> && is::Same<decltype(n13_2), decltype(n13_0)>);

      auto m22_0 = Mat<T, 2, 2>::template Constant<v3>();
      auto m22_1 = Mat<T, 2, 2>::template Constant<C3>();
      auto m22_2 = Mat<T, 2, 2>::template Constant<c3>();
      constexpr auto n22_0 = Mat<T, 2, 2>::template Constant<v3>();
      constexpr auto n22_1 = Mat<T, 2, 2>::template Constant<C3>();
      constexpr auto n22_2 = Mat<T, 2, 2>::template Constant<c3>();
      static_assert(is::Same<decltype(m22_0), Mat<T, 2, 2, Types<Types<C3, C3>, Types<C3, C3>>, Token>>);
      static_assert(is::Same<decltype(n22_0), Mat<T, 2, 2, Types<Types<C3, C3>, Types<C3, C3>>, Token> const>);
      static_assert(is::Same<decltype(m22_1), decltype(m22_0)> && is::Same<decltype(m22_2), decltype(m22_0)>);
      static_assert(is::Same<decltype(n22_1), decltype(n22_0)> && is::Same<decltype(n22_2), decltype(n22_0)>);
    });

    // `Zero` and `Identity`.
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;

      auto m11_z = Mat<T, 1, 1>::Zero();
      auto m21_z = Mat<T, 2, 1>::Zero();
      auto m31_z = Mat<T, 3, 1>::Zero();
      auto m12_z = Mat<T, 1, 2>::Zero();
      auto m13_z = Mat<T, 1, 3>::Zero();
      auto m22_z = Mat<T, 2, 2>::Zero();
      constexpr auto n11_z = Mat<T, 1, 1>::Zero();
      constexpr auto n21_z = Mat<T, 2, 1>::Zero();
      constexpr auto n31_z = Mat<T, 3, 1>::Zero();
      constexpr auto n12_z = Mat<T, 1, 2>::Zero();
      constexpr auto n13_z = Mat<T, 1, 3>::Zero();
      constexpr auto n22_z = Mat<T, 2, 2>::Zero();
      static_assert(is::Same<decltype(m11_z), Mat<T, 1, 1, Types<Types<C0>>, Token>>);
      static_assert(is::Same<decltype(n11_z), Mat<T, 1, 1, Types<Types<C0>>, Token> const>);
      static_assert(is::Same<decltype(m21_z), Mat<T, 2, 1, Types<Types<C0, C0>>, Token>>);
      static_assert(is::Same<decltype(n21_z), Mat<T, 2, 1, Types<Types<C0, C0>>, Token> const>);
      static_assert(is::Same<decltype(m31_z), Mat<T, 3, 1, Types<Types<C0, C0, C0>>, Token>>);
      static_assert(is::Same<decltype(n31_z), Mat<T, 3, 1, Types<Types<C0, C0, C0>>, Token> const>);
      static_assert(is::Same<decltype(m12_z), Mat<T, 1, 2, Types<Types<C0>, Types<C0>>, Token>>);
      static_assert(is::Same<decltype(n12_z), Mat<T, 1, 2, Types<Types<C0>, Types<C0>>, Token> const>);
      static_assert(is::Same<decltype(m13_z), Mat<T, 1, 3, Types<Types<C0>, Types<C0>, Types<C0>>, Token>>);
      static_assert(is::Same<decltype(n13_z), Mat<T, 1, 3, Types<Types<C0>, Types<C0>, Types<C0>>, Token> const>);
      static_assert(is::Same<decltype(m22_z), Mat<T, 2, 2, Types<Types<C0, C0>, Types<C0, C0>>, Token>>);
      static_assert(is::Same<decltype(n22_z), Mat<T, 2, 2, Types<Types<C0, C0>, Types<C0, C0>>, Token> const>);

      auto m11_i = Mat<T, 1, 1>::Identity();
      auto m22_i = Mat<T, 2, 2>::Identity();
      auto m33_i = Mat<T, 3, 3>::Identity();
      auto m44_i = Mat<T, 4, 4>::Identity();
      auto m55_i = Mat<T, 5, 5>::Identity();
      constexpr auto n11_i = Mat<T, 1, 1>::Identity();
      constexpr auto n22_i = Mat<T, 2, 2>::Identity();
      constexpr auto n33_i = Mat<T, 3, 3>::Identity();
      constexpr auto n44_i = Mat<T, 4, 4>::Identity();
      constexpr auto n55_i = Mat<T, 5, 5>::Identity();
      static_assert(is::Same<decltype(m11_i), Mat<T, 1, 1, Types<Types<C1>>, Token>>);
      static_assert(is::Same<decltype(n11_i), Mat<T, 1, 1, Types<Types<C1>>, Token> const>);
      static_assert(is::Same<decltype(m22_i), Mat<T, 2, 2, Types<Types<C1, C0>, Types<C0, C1>>, Token>>);
      static_assert(is::Same<decltype(n22_i), Mat<T, 2, 2, Types<Types<C1, C0>, Types<C0, C1>>, Token> const>);
      static_assert(
          is::Same<decltype(m33_i), Mat<T, 3, 3, Types<Types<C1, C0, C0>, Types<C0, C1, C0>, Types<C0, C0, C1>>, Token>>
      );
      static_assert(is::Same<
                    decltype(n33_i),
                    Mat<T, 3, 3, Types<Types<C1, C0, C0>, Types<C0, C1, C0>, Types<C0, C0, C1>>, Token> const>);
      static_assert(
          is::Same<
              decltype(m44_i),
              Mat<T, 4, 4,
                  Types<Types<C1, C0, C0, C0>, Types<C0, C1, C0, C0>, Types<C0, C0, C1, C0>, Types<C0, C0, C0, C1>>,
                  Token>>
      );
      static_assert(
          is::Same<
              decltype(n44_i),
              Mat<T, 4, 4,
                  Types<Types<C1, C0, C0, C0>, Types<C0, C1, C0, C0>, Types<C0, C0, C1, C0>, Types<C0, C0, C0, C1>>,
                  Token> const>
      );
      static_assert(
          is::Same<
              decltype(m55_i), Mat<T, 5, 5,
                                   Types<
                                       Types<C1, C0, C0, C0, C0>, Types<C0, C1, C0, C0, C0>, Types<C0, C0, C1, C0, C0>,
                                       Types<C0, C0, C0, C1, C0>, Types<C0, C0, C0, C0, C1>>,
                                   Token>>
      );
      static_assert(
          is::Same<
              decltype(n55_i), Mat<T, 5, 5,
                                   Types<
                                       Types<C1, C0, C0, C0, C0>, Types<C0, C1, C0, C0, C0>, Types<C0, C0, C1, C0, C0>,
                                       Types<C0, C0, C0, C1, C0>, Types<C0, C0, C0, C0, C1>>,
                                   Token> const>
      );
    });
  };

  //
  //
  //
  "Operators"_test = [] {
    ForEach<Types<f32>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      using I =
          std::conditional_t<is::Same<T, f32> || is::Same<T, f64>, std::conditional_t<is::Same<T, f32>, i32, i64>, T>;
      using I0 = C<static_cast<I>(0)>;
      using I1 = C<static_cast<I>(1)>;
      using I2 = C<static_cast<I>(2)>;
      using I3 = C<static_cast<I>(3)>;
      constexpr I0 i0;
      constexpr I1 i1;
      constexpr I2 i2;
      constexpr I3 i3;

      Mat m_VV_VV{Mat{T(2), T(3)}, Mat{T(5), T(7)}};
      Mat m_CV_VV{Mat{C<T(2)>{}, T(3)}, Mat{T(5), T(7)}};
      Mat m_VC_VV{Mat{T(2), C<T(3)>{}}, Mat{T(5), T(7)}};
      Mat m_CC_VV{Mat{C<T(2)>{}, C<T(3)>{}}, Mat{T(5), T(7)}};
      Mat m_VV_CV{Mat{T(2), T(3)}, Mat{C<T(5)>{}, T(7)}};
      Mat m_CV_CV{Mat{C<T(2)>{}, T(3)}, Mat{C<T(5)>{}, T(7)}};
      Mat m_VC_CV{Mat{T(2), C<T(3)>{}}, Mat{C<T(5)>{}, T(7)}};
      Mat m_CC_CV{Mat{C<T(2)>{}, C<T(3)>{}}, Mat{C<T(5)>{}, T(7)}};
      Mat m_VV_VC{Mat{T(2), T(3)}, Mat{T(5), C<T(7)>{}}};
      Mat m_CV_VC{Mat{C<T(2)>{}, T(3)}, Mat{T(5), C<T(7)>{}}};
      Mat m_VC_VC{Mat{T(2), C<T(3)>{}}, Mat{T(5), C<T(7)>{}}};
      Mat m_CC_VC{Mat{C<T(2)>{}, C<T(3)>{}}, Mat{T(5), C<T(7)>{}}};
      Mat m_VV_CC{Mat{T(2), T(3)}, Mat{C<T(5)>{}, C<T(7)>{}}};
      Mat m_CV_CC{Mat{C<T(2)>{}, T(3)}, Mat{C<T(5)>{}, C<T(7)>{}}};
      Mat m_VC_CC{Mat{T(2), C<T(3)>{}}, Mat{C<T(5)>{}, C<T(7)>{}}};
      Mat m_CC_CC{Mat{C<T(2)>{}, C<T(3)>{}}, Mat{C<T(5)>{}, C<T(7)>{}}};

      // Positive.
      {
        Mat VV_VV = +m_VV_VV;
        Mat VC_VV = +m_VC_VV;
        Mat VV_CV = +m_VV_CV;
        Mat VC_CV = +m_VC_CV;
        Mat VV_VC = +m_VV_VC;
        Mat VC_VC = +m_VC_VC;
        Mat VV_CC = +m_VV_CC;
        Mat VC_CC = +m_VC_CC;
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(3)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(5)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(3)>>, Types<C<T(5)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(7)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(3)>>, Types<VO, C<T(7)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(5)>, C<T(7)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(3)>>, Types<C<T(5)>, C<T(7)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == 2 && VV_VV[i1, i0] == 3 && VV_VV[i0, i1] == 5 && VV_VV[i1, i1] == 7);
        expect(VC_VV[i0, i0] == 2 && VC_VV[i1, i0] == 3 && VC_VV[i0, i1] == 5 && VC_VV[i1, i1] == 7);
        expect(VV_CV[i0, i0] == 2 && VV_CV[i1, i0] == 3 && VV_CV[i0, i1] == 5 && VV_CV[i1, i1] == 7);
        expect(VC_CV[i0, i0] == 2 && VC_CV[i1, i0] == 3 && VC_CV[i0, i1] == 5 && VC_CV[i1, i1] == 7);
        expect(VV_VC[i0, i0] == 2 && VV_VC[i1, i0] == 3 && VV_VC[i0, i1] == 5 && VV_VC[i1, i1] == 7);
        expect(VC_VC[i0, i0] == 2 && VC_VC[i1, i0] == 3 && VC_VC[i0, i1] == 5 && VC_VC[i1, i1] == 7);
        expect(VV_CC[i0, i0] == 2 && VV_CC[i1, i0] == 3 && VV_CC[i0, i1] == 5 && VV_CC[i1, i1] == 7);
        expect(VC_CC[i0, i0] == 2 && VC_CC[i1, i0] == 3 && VC_CC[i0, i1] == 5 && VC_CC[i1, i1] == 7);
      }

      // Negative.
      {
        Mat VV_VV = -m_VV_VV;
        Mat VC_VV = -m_VC_VV;
        Mat VV_CV = -m_VV_CV;
        Mat VC_CV = -m_VC_CV;
        Mat VV_VC = -m_VV_VC;
        Mat VC_VC = -m_VC_VC;
        Mat VV_CC = -m_VV_CC;
        Mat VC_CC = -m_VC_CC;
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(-3)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(-5)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(-3)>>, Types<C<T(-5)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(-7)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(-3)>>, Types<VO, C<T(-7)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(-5)>, C<T(-7)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(-3)>>, Types<C<T(-5)>, C<T(-7)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == -2 && VV_VV[i1, i0] == -3 && VV_VV[i0, i1] == -5 && VV_VV[i1, i1] == -7);
        expect(VC_VV[i0, i0] == -2 && VC_VV[i1, i0] == -3 && VC_VV[i0, i1] == -5 && VC_VV[i1, i1] == -7);
        expect(VV_CV[i0, i0] == -2 && VV_CV[i1, i0] == -3 && VV_CV[i0, i1] == -5 && VV_CV[i1, i1] == -7);
        expect(VC_CV[i0, i0] == -2 && VC_CV[i1, i0] == -3 && VC_CV[i0, i1] == -5 && VC_CV[i1, i1] == -7);
        expect(VV_VC[i0, i0] == -2 && VV_VC[i1, i0] == -3 && VV_VC[i0, i1] == -5 && VV_VC[i1, i1] == -7);
        expect(VC_VC[i0, i0] == -2 && VC_VC[i1, i0] == -3 && VC_VC[i0, i1] == -5 && VC_VC[i1, i1] == -7);
        expect(VV_CC[i0, i0] == -2 && VV_CC[i1, i0] == -3 && VV_CC[i0, i1] == -5 && VV_CC[i1, i1] == -7);
        expect(VC_CC[i0, i0] == -2 && VC_CC[i1, i0] == -3 && VC_CC[i0, i1] == -5 && VC_CC[i1, i1] == -7);
      }

      // Equal to.
      {
        expect(m_VV_VV == m_CV_VV);
        expect(m_VC_VV == m_CC_VV);
        expect(m_VV_CV == m_CV_CV);
        expect(m_VC_CV == m_CC_CV);
        expect(m_VV_VC == m_CV_VC);
        expect(m_VC_VC == m_CC_VC);
        expect(m_VV_CC == m_CV_CC);
        expect(m_VC_CC == m_CC_CC);
      }

      // Unequal to.
      {
        expect(m_VV_VV != -m_CV_VV);
        expect(m_VC_VV != -m_CC_VV);
        expect(m_VV_CV != -m_CV_CV);
        expect(m_VC_CV != -m_CC_CV);
        expect(m_VV_VC != -m_CV_VC);
        expect(m_VC_VC != -m_CC_VC);
        expect(m_VV_CC != -m_CV_CC);
        expect(m_VC_CC != -m_CC_CC);
      }

      // Add.
      {
        Mat VV_VV = m_VV_VV + m_CV_VV;
        Mat VC_VV = m_VC_VV + m_CC_VV;
        Mat VV_CV = m_VV_CV + m_CV_CV;
        Mat VC_CV = m_VC_CV + m_CC_CV;
        Mat VV_VC = m_VV_VC + m_CV_VC;
        Mat VC_VC = m_VC_VC + m_CC_VC;
        Mat VV_CC = m_VV_CC + m_CV_CC;
        Mat VC_CC = m_VC_CC + m_CC_CC;
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(6)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(10)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(6)>>, Types<C<T(10)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(14)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(6)>>, Types<VO, C<T(14)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(10)>, C<T(14)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(6)>>, Types<C<T(10)>, C<T(14)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == 4 && VV_VV[i1, i0] == 6 && VV_VV[i0, i1] == 10 && VV_VV[i1, i1] == 14);
        expect(VC_VV[i0, i0] == 4 && VC_VV[i1, i0] == 6 && VC_VV[i0, i1] == 10 && VC_VV[i1, i1] == 14);
        expect(VV_CV[i0, i0] == 4 && VV_CV[i1, i0] == 6 && VV_CV[i0, i1] == 10 && VV_CV[i1, i1] == 14);
        expect(VC_CV[i0, i0] == 4 && VC_CV[i1, i0] == 6 && VC_CV[i0, i1] == 10 && VC_CV[i1, i1] == 14);
        expect(VV_VC[i0, i0] == 4 && VV_VC[i1, i0] == 6 && VV_VC[i0, i1] == 10 && VV_VC[i1, i1] == 14);
        expect(VC_VC[i0, i0] == 4 && VC_VC[i1, i0] == 6 && VC_VC[i0, i1] == 10 && VC_VC[i1, i1] == 14);
        expect(VV_CC[i0, i0] == 4 && VV_CC[i1, i0] == 6 && VV_CC[i0, i1] == 10 && VV_CC[i1, i1] == 14);
        expect(VC_CC[i0, i0] == 4 && VC_CC[i1, i0] == 6 && VC_CC[i0, i1] == 10 && VC_CC[i1, i1] == 14);

        VC_CC += Mat{Mat{c3, c0}, Mat{c0, c0}};
        expect(VC_CC[i0, i0] == 7 && VC_CC[i1, i0] == 6 && VC_CC[i0, i1] == 10 && VC_CC[i1, i1] == 14);
      }

      // Subtract.
      {
        Mat VV_VV = m_VV_VV - m_CV_VV;
        Mat VC_VV = m_VC_VV - m_CC_VV;
        Mat VV_CV = m_VV_CV - m_CV_CV;
        Mat VC_CV = m_VC_CV - m_CC_CV;
        Mat VV_VC = m_VV_VC - m_CV_VC;
        Mat VC_VC = m_VC_VC - m_CC_VC;
        Mat VV_CC = m_VV_CC - m_CV_CC;
        Mat VC_CC = m_VC_CC - m_CC_CC;
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(0)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(0)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(0)>>, Types<C<T(0)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(0)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(0)>>, Types<VO, C<T(0)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(0)>, C<T(0)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(0)>>, Types<C<T(0)>, C<T(0)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == 0 && VV_VV[i1, i0] == 0 && VV_VV[i0, i1] == 0 && VV_VV[i1, i1] == 0);
        expect(VC_VV[i0, i0] == 0 && VC_VV[i1, i0] == 0 && VC_VV[i0, i1] == 0 && VC_VV[i1, i1] == 0);
        expect(VV_CV[i0, i0] == 0 && VV_CV[i1, i0] == 0 && VV_CV[i0, i1] == 0 && VV_CV[i1, i1] == 0);
        expect(VC_CV[i0, i0] == 0 && VC_CV[i1, i0] == 0 && VC_CV[i0, i1] == 0 && VC_CV[i1, i1] == 0);
        expect(VV_VC[i0, i0] == 0 && VV_VC[i1, i0] == 0 && VV_VC[i0, i1] == 0 && VV_VC[i1, i1] == 0);
        expect(VC_VC[i0, i0] == 0 && VC_VC[i1, i0] == 0 && VC_VC[i0, i1] == 0 && VC_VC[i1, i1] == 0);
        expect(VV_CC[i0, i0] == 0 && VV_CC[i1, i0] == 0 && VV_CC[i0, i1] == 0 && VV_CC[i1, i1] == 0);
        expect(VC_CC[i0, i0] == 0 && VC_CC[i1, i0] == 0 && VC_CC[i0, i1] == 0 && VC_CC[i1, i1] == 0);

        VC_CC -= Mat{Mat{c3, c0}, Mat{c0, c0}};
        expect(VC_CC[i0, i0] == -3 && VC_CC[i1, i0] == 0 && VC_CC[i0, i1] == 0 && VC_CC[i1, i1] == 0);
      }

      // Multiply.
      {
        Mat VV_VV = m_VV_VV * C<T(3)>{};
        Mat VC_VV = m_VC_VV * C<T(3)>{};
        Mat VV_CV = m_VV_CV * C<T(3)>{};
        Mat VC_CV = m_VC_CV * C<T(3)>{};
        Mat VV_VC = m_VV_VC * C<T(3)>{};
        Mat VC_VC = m_VC_VC * C<T(3)>{};
        Mat VV_CC = m_VV_CC * C<T(3)>{};
        Mat VC_CC = m_VC_CC * C<T(3)>{};
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(15)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<C<T(15)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(21)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<VO, C<T(21)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(15)>, C<T(21)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<C<T(15)>, C<T(21)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == 6 && VV_VV[i1, i0] == 9 && VV_VV[i0, i1] == 15 && VV_VV[i1, i1] == 21);
        expect(VC_VV[i0, i0] == 6 && VC_VV[i1, i0] == 9 && VC_VV[i0, i1] == 15 && VC_VV[i1, i1] == 21);
        expect(VV_CV[i0, i0] == 6 && VV_CV[i1, i0] == 9 && VV_CV[i0, i1] == 15 && VV_CV[i1, i1] == 21);
        expect(VC_CV[i0, i0] == 6 && VC_CV[i1, i0] == 9 && VC_CV[i0, i1] == 15 && VC_CV[i1, i1] == 21);
        expect(VV_VC[i0, i0] == 6 && VV_VC[i1, i0] == 9 && VV_VC[i0, i1] == 15 && VV_VC[i1, i1] == 21);
        expect(VC_VC[i0, i0] == 6 && VC_VC[i1, i0] == 9 && VC_VC[i0, i1] == 15 && VC_VC[i1, i1] == 21);
        expect(VV_CC[i0, i0] == 6 && VV_CC[i1, i0] == 9 && VV_CC[i0, i1] == 15 && VV_CC[i1, i1] == 21);
        expect(VC_CC[i0, i0] == 6 && VC_CC[i1, i0] == 9 && VC_CC[i0, i1] == 15 && VC_CC[i1, i1] == 21);

        VV_VV *= C<3>{};
        expect(VV_VV[i0, i0] == 18 && VV_VV[i1, i0] == 27 && VV_VV[i0, i1] == 45 && VV_VV[i1, i1] == 63);
      }

      {
        Mat VV_VV = C<T(3)>{} * m_VV_VV;
        Mat VC_VV = C<T(3)>{} * m_VC_VV;
        Mat VV_CV = C<T(3)>{} * m_VV_CV;
        Mat VC_CV = C<T(3)>{} * m_VC_CV;
        Mat VV_VC = C<T(3)>{} * m_VV_VC;
        Mat VC_VC = C<T(3)>{} * m_VC_VC;
        Mat VV_CC = C<T(3)>{} * m_VV_CC;
        Mat VC_CC = C<T(3)>{} * m_VC_CC;
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(15)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<C<T(15)>, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(21)>>>, Token>>);
        static_assert(is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<VO, C<T(21)>>>, Token>>);
        static_assert(is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(15)>, C<T(21)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(9)>>, Types<C<T(15)>, C<T(21)>>>, Token>>
        );
        expect(VV_VV[i0, i0] == 6 && VV_VV[i1, i0] == 9 && VV_VV[i0, i1] == 15 && VV_VV[i1, i1] == 21);
        expect(VC_VV[i0, i0] == 6 && VC_VV[i1, i0] == 9 && VC_VV[i0, i1] == 15 && VC_VV[i1, i1] == 21);
        expect(VV_CV[i0, i0] == 6 && VV_CV[i1, i0] == 9 && VV_CV[i0, i1] == 15 && VV_CV[i1, i1] == 21);
        expect(VC_CV[i0, i0] == 6 && VC_CV[i1, i0] == 9 && VC_CV[i0, i1] == 15 && VC_CV[i1, i1] == 21);
        expect(VV_VC[i0, i0] == 6 && VV_VC[i1, i0] == 9 && VV_VC[i0, i1] == 15 && VV_VC[i1, i1] == 21);
        expect(VC_VC[i0, i0] == 6 && VC_VC[i1, i0] == 9 && VC_VC[i0, i1] == 15 && VC_VC[i1, i1] == 21);
        expect(VV_CC[i0, i0] == 6 && VV_CC[i1, i0] == 9 && VV_CC[i0, i1] == 15 && VV_CC[i1, i1] == 21);
        expect(VC_CC[i0, i0] == 6 && VC_CC[i1, i0] == 9 && VC_CC[i0, i1] == 15 && VC_CC[i1, i1] == 21);
      }

      // Divide.
      {
        Mat VV_VV = m_VV_VV / C<T(3)>{};
        Mat VC_VV = m_VC_VV / C<T(3)>{};
        Mat VV_CV = m_VV_CV / C<T(3)>{};
        Mat VC_CV = m_VC_CV / C<T(3)>{};
        Mat VV_VC = m_VV_VC / C<T(3)>{};
        Mat VC_VC = m_VC_VC / C<T(3)>{};
        Mat VV_CC = m_VV_CC / C<T(3)>{};
        Mat VC_CC = m_VC_CC / C<T(3)>{};
        static_assert(is::Same<decltype(VV_VV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VC_VV), Mat<T, 2, 2, Types<Types<VO, C<T(1)>>, Types<VO, VO>>, Token>>);
        static_assert(is::Same<decltype(VV_CV), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(5) / T(3)>, VO>>, Token>>);
        static_assert(
            is::Same<decltype(VC_CV), Mat<T, 2, 2, Types<Types<VO, C<T(1)>>, Types<C<T(5) / T(3)>, VO>>, Token>>
        );
        static_assert(is::Same<decltype(VV_VC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<VO, C<T(7) / T(3)>>>, Token>>);
        static_assert(
            is::Same<decltype(VC_VC), Mat<T, 2, 2, Types<Types<VO, C<T(1)>>, Types<VO, C<T(7) / T(3)>>>, Token>>
        );
        static_assert(
            is::Same<decltype(VV_CC), Mat<T, 2, 2, Types<Types<VO, VO>, Types<C<T(5) / T(3)>, C<T(7) / T(3)>>>, Token>>
        );
        static_assert(
            is::Same<
                decltype(VC_CC), Mat<T, 2, 2, Types<Types<VO, C<T(1)>>, Types<C<T(5) / T(3)>, C<T(7) / T(3)>>>, Token>>
        );
        expect(
            VV_VV[i0, i0] == T(2) / T(3) && VV_VV[i1, i0] == 1 && VV_VV[i0, i1] == T(5) / T(3) &&
            VV_VV[i1, i1] == T(7) / T(3)
        );
        expect(
            VC_VV[i0, i0] == T(2) / T(3) && VC_VV[i1, i0] == 1 && VC_VV[i0, i1] == T(5) / T(3) &&
            VC_VV[i1, i1] == T(7) / T(3)
        );
        expect(
            VV_CV[i0, i0] == T(2) / T(3) && VV_CV[i1, i0] == 1 && VV_CV[i0, i1] == T(5) / T(3) &&
            VV_CV[i1, i1] == T(7) / T(3)
        );
        expect(
            VC_CV[i0, i0] == T(2) / T(3) && VC_CV[i1, i0] == 1 && VC_CV[i0, i1] == T(5) / T(3) &&
            VC_CV[i1, i1] == T(7) / T(3)
        );
        expect(
            VV_VC[i0, i0] == T(2) / T(3) && VV_VC[i1, i0] == 1 && VV_VC[i0, i1] == T(5) / T(3) &&
            VV_VC[i1, i1] == T(7) / T(3)
        );
        expect(
            VC_VC[i0, i0] == T(2) / T(3) && VC_VC[i1, i0] == 1 && VC_VC[i0, i1] == T(5) / T(3) &&
            VC_VC[i1, i1] == T(7) / T(3)
        );
        expect(
            VV_CC[i0, i0] == T(2) / T(3) && VV_CC[i1, i0] == 1 && VV_CC[i0, i1] == T(5) / T(3) &&
            VV_CC[i1, i1] == T(7) / T(3)
        );
        expect(
            VC_CC[i0, i0] == T(2) / T(3) && VC_CC[i1, i0] == 1 && VC_CC[i0, i1] == T(5) / T(3) &&
            VC_CC[i1, i1] == T(7) / T(3)
        );

        VV_VV /= C<3>{};
        expect(
            VV_VV[i0, i0] == T(2) / T(3) / T(3) && VV_VV[i1, i0] == T(1) / T(3) &&
            VV_VV[i0, i1] == T(5) / T(3) / T(3) && VV_VV[i1, i1] == T(7) / T(3) / T(3)
        );
      }
    });
  };

  //
  //
  //
  "Aliases"_test = [] {
    ForEach<Types<i8, u8, i16, u16, i32, u32, i64, u64, isize, usize, f32, f64>>([]<typename T> {
      using VO = void;
      T u0(0), u1(1), u2(2), u3(3);
      constexpr T v0(0), v1(1), v2(2), v3(3);
      using C0 = C<v0>;
      using C1 = C<v1>;
      using C2 = C<v2>;
      using C3 = C<v3>;
      constexpr C0 c0;
      constexpr C1 c1;
      constexpr C2 c2;
      constexpr C3 c3;

      static_assert(is::Same<Mat1<T>, Mat<T, 1, 1>>);
      static_assert(is::Same<Mat2<T>, Mat<T, 2, 2>>);
      static_assert(is::Same<Mat3<T>, Mat<T, 3, 3>>);
      static_assert(is::Same<Mat4<T>, Mat<T, 4, 4>>);
      static_assert(is::Same<Mat1<T, Types<Types<C1>>, Token>, decltype(Mat1<T>::Identity())>);
      static_assert(is::Same<Mat2<T, Types<Types<C1, C0>, Types<C0, C1>>, Token>, decltype(Mat2<T>::Identity())>);
      static_assert(is::Same<
                    Mat3<T, Types<Types<C1, C0, C0>, Types<C0, C1, C0>, Types<C0, C0, C1>>, Token>,
                    decltype(Mat3<T>::Identity())>);
      static_assert(
          is::Same<
              Mat4<
                  T, Types<Types<C1, C0, C0, C0>, Types<C0, C1, C0, C0>, Types<C0, C0, C1, C0>, Types<C0, C0, C0, C1>>,
                  Token>,
              decltype(Mat4<T>::Identity())>
      );

      static_assert(is::Same<Vec1<T>, Mat<T, 1, 1>>);
      static_assert(is::Same<Vec2<T>, Mat<T, 2, 1>>);
      static_assert(is::Same<Vec3<T>, Mat<T, 3, 1>>);
      static_assert(is::Same<Vec4<T>, Mat<T, 4, 1>>);
      static_assert(is::Same<Vec1<T, Types<Types<C0>>, Token>, decltype(Vec1<T>::Zero())>);
      static_assert(is::Same<Vec2<T, Types<Types<C0, C0>>, Token>, decltype(Vec2<T>::Zero())>);
      static_assert(is::Same<Vec3<T, Types<Types<C0, C0, C0>>, Token>, decltype(Vec3<T>::Zero())>);
      static_assert(is::Same<Vec4<T, Types<Types<C0, C0, C0, C0>>, Token>, decltype(Vec4<T>::Zero())>);
    });

    using C0 = C<0.0F>;
    static_assert(is::Same<Vec1f<>, Mat<float, 1, 1>>);
    static_assert(is::Same<Vec2f<>, Mat<float, 2, 1>>);
    static_assert(is::Same<Vec3f<>, Mat<float, 3, 1>>);
    static_assert(is::Same<Vec4f<>, Mat<float, 4, 1>>);
    static_assert(is::Same<Vec1<float, Types<Types<C0>>, Token>, decltype(Vec1f<>::Zero())>);
    static_assert(is::Same<Vec2<float, Types<Types<C0, C0>>, Token>, decltype(Vec2f<>::Zero())>);
    static_assert(is::Same<Vec3<float, Types<Types<C0, C0, C0>>, Token>, decltype(Vec3f<>::Zero())>);
    static_assert(is::Same<Vec4<float, Types<Types<C0, C0, C0, C0>>, Token>, decltype(Vec4f<>::Zero())>);
  };
};

} // namespace
