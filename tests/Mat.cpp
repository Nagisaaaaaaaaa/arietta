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
          static_assert(n[0, 0] == 0);
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
            static_assert(n[0, 0] == 0);
            static_assert(n[1, 0] == 0);
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
              static_assert(n[0, 0] == 0);
              static_assert(n[1, 0] == 0);
              static_assert(n[2, 0] == 0);
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
  };

  //
  //
  //
  "Constants"_test = [] {
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
  };
};

} // namespace
