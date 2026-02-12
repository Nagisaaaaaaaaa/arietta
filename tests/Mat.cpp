#include "Arietta/Mat.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

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
  "Instantiations"_test = [] {
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

      Mat m_V{u0};
      Mat m_C{c0};
      constexpr Mat n_V{v0};
      constexpr Mat n_C{c0};
      static_assert(is::Same<decltype(m_V), Mat<T, Types<Types<VO>>>>);
      static_assert(is::Same<decltype(m_C), Mat<T, Types<Types<C0>>>>);
      static_assert(is::Same<decltype(n_V), Mat<T, Types<Types<VO>>> const>);
      static_assert(is::Same<decltype(n_C), Mat<T, Types<Types<C0>>> const>);

      Mat m_VV{u0, u1};
      Mat m_CV{c0, u1};
      Mat m_VC{u0, c1};
      Mat m_CC{c0, c1};
      constexpr Mat n_VV{v0, v1};
      constexpr Mat n_CV{c0, v1};
      constexpr Mat n_VC{v0, c1};
      constexpr Mat n_CC{c0, c1};
      static_assert(is::Same<decltype(m_VV), Mat<T, Types<Types<VO, VO>>>>);
      static_assert(is::Same<decltype(m_CV), Mat<T, Types<Types<C0, VO>>>>);
      static_assert(is::Same<decltype(m_VC), Mat<T, Types<Types<VO, C1>>>>);
      static_assert(is::Same<decltype(m_CC), Mat<T, Types<Types<C0, C1>>>>);
      static_assert(is::Same<decltype(n_VV), Mat<T, Types<Types<VO, VO>>> const>);
      static_assert(is::Same<decltype(n_CV), Mat<T, Types<Types<C0, VO>>> const>);
      static_assert(is::Same<decltype(n_VC), Mat<T, Types<Types<VO, C1>>> const>);
      static_assert(is::Same<decltype(n_CC), Mat<T, Types<Types<C0, C1>>> const>);

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
      static_assert(is::Same<decltype(m_VVV), Mat<T, Types<Types<VO, VO, VO>>>>);
      static_assert(is::Same<decltype(m_CVV), Mat<T, Types<Types<C0, VO, VO>>>>);
      static_assert(is::Same<decltype(m_VCV), Mat<T, Types<Types<VO, C1, VO>>>>);
      static_assert(is::Same<decltype(m_CCV), Mat<T, Types<Types<C0, C1, VO>>>>);
      static_assert(is::Same<decltype(m_VVC), Mat<T, Types<Types<VO, VO, C2>>>>);
      static_assert(is::Same<decltype(m_CVC), Mat<T, Types<Types<C0, VO, C2>>>>);
      static_assert(is::Same<decltype(m_VCC), Mat<T, Types<Types<VO, C1, C2>>>>);
      static_assert(is::Same<decltype(m_CCC), Mat<T, Types<Types<C0, C1, C2>>>>);
      static_assert(is::Same<decltype(n_VVV), Mat<T, Types<Types<VO, VO, VO>>> const>);
      static_assert(is::Same<decltype(n_CVV), Mat<T, Types<Types<C0, VO, VO>>> const>);
      static_assert(is::Same<decltype(n_VCV), Mat<T, Types<Types<VO, C1, VO>>> const>);
      static_assert(is::Same<decltype(n_CCV), Mat<T, Types<Types<C0, C1, VO>>> const>);
      static_assert(is::Same<decltype(n_VVC), Mat<T, Types<Types<VO, VO, C2>>> const>);
      static_assert(is::Same<decltype(n_CVC), Mat<T, Types<Types<C0, VO, C2>>> const>);
      static_assert(is::Same<decltype(n_VCC), Mat<T, Types<Types<VO, C1, C2>>> const>);
      static_assert(is::Same<decltype(n_CCC), Mat<T, Types<Types<C0, C1, C2>>> const>);

      Mat m_V_V{Mat{u0}, Mat{u1}};
      Mat m_C_V{Mat{c0}, Mat{u1}};
      Mat m_V_C{Mat{u0}, Mat{c1}};
      Mat m_C_C{Mat{c0}, Mat{c1}};
      constexpr Mat n_V_V{Mat{v0}, Mat{v1}};
      constexpr Mat n_C_V{Mat{c0}, Mat{v1}};
      constexpr Mat n_V_C{Mat{v0}, Mat{c1}};
      constexpr Mat n_C_C{Mat{c0}, Mat{c1}};
      static_assert(is::Same<decltype(m_V_V), Mat<T, Types<Types<VO>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_C_V), Mat<T, Types<Types<C0>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_V_C), Mat<T, Types<Types<VO>, Types<C1>>>>);
      static_assert(is::Same<decltype(m_C_C), Mat<T, Types<Types<C0>, Types<C1>>>>);
      static_assert(is::Same<decltype(n_V_V), Mat<T, Types<Types<VO>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_C_V), Mat<T, Types<Types<C0>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_V_C), Mat<T, Types<Types<VO>, Types<C1>>> const>);
      static_assert(is::Same<decltype(n_C_C), Mat<T, Types<Types<C0>, Types<C1>>> const>);

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
      static_assert(is::Same<decltype(m_VV_VV), Mat<T, Types<Types<VO, VO>, Types<VO, VO>>>>);
      static_assert(is::Same<decltype(m_CV_VV), Mat<T, Types<Types<C0, VO>, Types<VO, VO>>>>);
      static_assert(is::Same<decltype(m_VC_VV), Mat<T, Types<Types<VO, C1>, Types<VO, VO>>>>);
      static_assert(is::Same<decltype(m_CC_VV), Mat<T, Types<Types<C0, C1>, Types<VO, VO>>>>);
      static_assert(is::Same<decltype(m_VV_CV), Mat<T, Types<Types<VO, VO>, Types<C2, VO>>>>);
      static_assert(is::Same<decltype(m_CV_CV), Mat<T, Types<Types<C0, VO>, Types<C2, VO>>>>);
      static_assert(is::Same<decltype(m_VC_CV), Mat<T, Types<Types<VO, C1>, Types<C2, VO>>>>);
      static_assert(is::Same<decltype(m_CC_CV), Mat<T, Types<Types<C0, C1>, Types<C2, VO>>>>);
      static_assert(is::Same<decltype(m_VV_VC), Mat<T, Types<Types<VO, VO>, Types<VO, C3>>>>);
      static_assert(is::Same<decltype(m_CV_VC), Mat<T, Types<Types<C0, VO>, Types<VO, C3>>>>);
      static_assert(is::Same<decltype(m_VC_VC), Mat<T, Types<Types<VO, C1>, Types<VO, C3>>>>);
      static_assert(is::Same<decltype(m_CC_VC), Mat<T, Types<Types<C0, C1>, Types<VO, C3>>>>);
      static_assert(is::Same<decltype(m_VV_CC), Mat<T, Types<Types<VO, VO>, Types<C2, C3>>>>);
      static_assert(is::Same<decltype(m_CV_CC), Mat<T, Types<Types<C0, VO>, Types<C2, C3>>>>);
      static_assert(is::Same<decltype(m_VC_CC), Mat<T, Types<Types<VO, C1>, Types<C2, C3>>>>);
      static_assert(is::Same<decltype(m_CC_CC), Mat<T, Types<Types<C0, C1>, Types<C2, C3>>>>);
      static_assert(is::Same<decltype(n_VV_VV), Mat<T, Types<Types<VO, VO>, Types<VO, VO>>> const>);
      static_assert(is::Same<decltype(n_CV_VV), Mat<T, Types<Types<C0, VO>, Types<VO, VO>>> const>);
      static_assert(is::Same<decltype(n_VC_VV), Mat<T, Types<Types<VO, C1>, Types<VO, VO>>> const>);
      static_assert(is::Same<decltype(n_CC_VV), Mat<T, Types<Types<C0, C1>, Types<VO, VO>>> const>);
      static_assert(is::Same<decltype(n_VV_CV), Mat<T, Types<Types<VO, VO>, Types<C2, VO>>> const>);
      static_assert(is::Same<decltype(n_CV_CV), Mat<T, Types<Types<C0, VO>, Types<C2, VO>>> const>);
      static_assert(is::Same<decltype(n_VC_CV), Mat<T, Types<Types<VO, C1>, Types<C2, VO>>> const>);
      static_assert(is::Same<decltype(n_CC_CV), Mat<T, Types<Types<C0, C1>, Types<C2, VO>>> const>);
      static_assert(is::Same<decltype(n_VV_VC), Mat<T, Types<Types<VO, VO>, Types<VO, C3>>> const>);
      static_assert(is::Same<decltype(n_CV_VC), Mat<T, Types<Types<C0, VO>, Types<VO, C3>>> const>);
      static_assert(is::Same<decltype(n_VC_VC), Mat<T, Types<Types<VO, C1>, Types<VO, C3>>> const>);
      static_assert(is::Same<decltype(n_CC_VC), Mat<T, Types<Types<C0, C1>, Types<VO, C3>>> const>);
      static_assert(is::Same<decltype(n_VV_CC), Mat<T, Types<Types<VO, VO>, Types<C2, C3>>> const>);
      static_assert(is::Same<decltype(n_CV_CC), Mat<T, Types<Types<C0, VO>, Types<C2, C3>>> const>);
      static_assert(is::Same<decltype(n_VC_CC), Mat<T, Types<Types<VO, C1>, Types<C2, C3>>> const>);
      static_assert(is::Same<decltype(n_CC_CC), Mat<T, Types<Types<C0, C1>, Types<C2, C3>>> const>);

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
      static_assert(is::Same<decltype(m_V_V_V), Mat<T, Types<Types<VO>, Types<VO>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_C_V_V), Mat<T, Types<Types<C0>, Types<VO>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_V_C_V), Mat<T, Types<Types<VO>, Types<C1>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_C_C_V), Mat<T, Types<Types<C0>, Types<C1>, Types<VO>>>>);
      static_assert(is::Same<decltype(m_V_V_C), Mat<T, Types<Types<VO>, Types<VO>, Types<C2>>>>);
      static_assert(is::Same<decltype(m_C_V_C), Mat<T, Types<Types<C0>, Types<VO>, Types<C2>>>>);
      static_assert(is::Same<decltype(m_V_C_C), Mat<T, Types<Types<VO>, Types<C1>, Types<C2>>>>);
      static_assert(is::Same<decltype(m_C_C_C), Mat<T, Types<Types<C0>, Types<C1>, Types<C2>>>>);
      static_assert(is::Same<decltype(n_V_V_V), Mat<T, Types<Types<VO>, Types<VO>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_C_V_V), Mat<T, Types<Types<C0>, Types<VO>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_V_C_V), Mat<T, Types<Types<VO>, Types<C1>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_C_C_V), Mat<T, Types<Types<C0>, Types<C1>, Types<VO>>> const>);
      static_assert(is::Same<decltype(n_V_V_C), Mat<T, Types<Types<VO>, Types<VO>, Types<C2>>> const>);
      static_assert(is::Same<decltype(n_C_V_C), Mat<T, Types<Types<C0>, Types<VO>, Types<C2>>> const>);
      static_assert(is::Same<decltype(n_V_C_C), Mat<T, Types<Types<VO>, Types<C1>, Types<C2>>> const>);
      static_assert(is::Same<decltype(n_C_C_C), Mat<T, Types<Types<C0>, Types<C1>, Types<C2>>> const>);
    });
  };
};

} // namespace
