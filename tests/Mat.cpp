#include "Arietta/Mat.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Mat"> _ = [] {
  "Mat"_test = [] {
    using T = int;

    {
      using M = Mat<T, 2, 3, Types<Types<void, void, void>, Types<void, void, void>>>;
      constexpr auto i = M::coords2Indices();
      constexpr auto n = M::nValuesToStore();
      static_assert(i[0][0] == 0 & i[0][1] == 1 & i[0][2] == 2 & i[1][0] == 3 & i[1][1] == 4 & i[1][2] == 5);
      static_assert(n == 6);
      static_assert(sizeof(M) == n * sizeof(T));
    }

    {
      using M0 = Mat<T, 2, 3, Types<Types<C<0>, void, void>, Types<void, void, void>>>;
      using M1 = Mat<T, 2, 3, Types<Types<void, C<1>, void>, Types<void, void, void>>>;
      using M2 = Mat<T, 2, 3, Types<Types<void, void, C<2>>, Types<void, void, void>>>;
      using M3 = Mat<T, 2, 3, Types<Types<void, void, void>, Types<C<3>, void, void>>>;
      using M4 = Mat<T, 2, 3, Types<Types<void, void, void>, Types<void, C<4>, void>>>;
      using M5 = Mat<T, 2, 3, Types<Types<void, void, void>, Types<void, void, C<5>>>>;
      constexpr auto i0 = M0::coords2Indices(), i1 = M1::coords2Indices(), i2 = M2::coords2Indices(),
                     i3 = M3::coords2Indices(), i4 = M4::coords2Indices(), i5 = M5::coords2Indices();
      constexpr auto n0 = M0::nValuesToStore(), n1 = M1::nValuesToStore(), n2 = M2::nValuesToStore(),
                     n3 = M3::nValuesToStore(), n4 = M4::nValuesToStore(), n5 = M5::nValuesToStore();
      static_assert(i0[0][0] == -1 & i0[0][1] == 0 & i0[0][2] == 1 & i0[1][0] == 2 & i0[1][1] == 3 & i0[1][2] == 4);
      static_assert(i1[0][0] == 0 & i1[0][1] == -1 & i1[0][2] == 1 & i1[1][0] == 2 & i1[1][1] == 3 & i1[1][2] == 4);
      static_assert(i2[0][0] == 0 & i2[0][1] == 1 & i2[0][2] == -1 & i2[1][0] == 2 & i2[1][1] == 3 & i2[1][2] == 4);
      static_assert(i3[0][0] == 0 & i3[0][1] == 1 & i3[0][2] == 2 & i3[1][0] == -1 & i3[1][1] == 3 & i3[1][2] == 4);
      static_assert(i4[0][0] == 0 & i4[0][1] == 1 & i4[0][2] == 2 & i4[1][0] == 3 & i4[1][1] == -1 & i4[1][2] == 4);
      static_assert(i5[0][0] == 0 & i5[0][1] == 1 & i5[0][2] == 2 & i5[1][0] == 3 & i5[1][1] == 4 & i5[1][2] == -1);
      static_assert(n0 == 5 && n1 == 5 && n2 == 5 && n3 == 5 && n4 == 5 && n5 == 5);
      static_assert(
          sizeof(M0) == n0 * sizeof(T) && sizeof(M1) == n1 * sizeof(T) && sizeof(M2) == n2 * sizeof(T) &&
          sizeof(M3) == n3 * sizeof(T) && sizeof(M4) == n4 * sizeof(T) && sizeof(M5) == n5 * sizeof(T)
      );
    }

    {
      using M0 = Mat<T, 2, 3, Types<Types<C<0>, void, void>, Types<void, void, void>>>;
      using M1 = Mat<T, 2, 3, Types<Types<C<0>, C<1>, void>, Types<void, void, void>>>;
      using M2 = Mat<T, 2, 3, Types<Types<C<0>, C<1>, C<2>>, Types<void, void, void>>>;
      using M3 = Mat<T, 2, 3, Types<Types<C<0>, C<1>, C<2>>, Types<C<3>, void, void>>>;
      using M4 = Mat<T, 2, 3, Types<Types<C<0>, C<1>, C<2>>, Types<C<3>, C<4>, void>>>;
      using M5 = Mat<T, 2, 3, Types<Types<C<0>, C<1>, C<2>>, Types<C<3>, C<4>, C<5>>>>;
      constexpr auto i0 = M0::coords2Indices(), i1 = M1::coords2Indices(), i2 = M2::coords2Indices(),
                     i3 = M3::coords2Indices(), i4 = M4::coords2Indices(), i5 = M5::coords2Indices();
      constexpr auto n0 = M0::nValuesToStore(), n1 = M1::nValuesToStore(), n2 = M2::nValuesToStore(),
                     n3 = M3::nValuesToStore(), n4 = M4::nValuesToStore(), n5 = M5::nValuesToStore();
      static_assert(i0[0][0] == -1 & i0[0][1] == 0 & i0[0][2] == 1 & i0[1][0] == 2 & i0[1][1] == 3 & i0[1][2] == 4);
      static_assert(i1[0][0] == -1 & i1[0][1] == -1 & i1[0][2] == 0 & i1[1][0] == 1 & i1[1][1] == 2 & i1[1][2] == 3);
      static_assert(i2[0][0] == -1 & i2[0][1] == -1 & i2[0][2] == -1 & i2[1][0] == 0 & i2[1][1] == 1 & i2[1][2] == 2);
      static_assert(i3[0][0] == -1 & i3[0][1] == -1 & i3[0][2] == -1 & i3[1][0] == -1 & i3[1][1] == 0 & i3[1][2] == 1);
      static_assert(i4[0][0] == -1 & i4[0][1] == -1 & i4[0][2] == -1 & i4[1][0] == -1 & i4[1][1] == -1 & i4[1][2] == 0);
      static_assert(
          i5[0][0] == -1 & i5[0][1] == -1 & i5[0][2] == -1 & i5[1][0] == -1 & i5[1][1] == -1 & i5[1][2] == -1
      );
      static_assert(n0 == 5 && n1 == 4 && n2 == 3 && n3 == 2 && n4 == 1 && n5 == 0);
      static_assert(
          sizeof(M0) == n0 * sizeof(T) && sizeof(M1) == n1 * sizeof(T) && sizeof(M2) == n2 * sizeof(T) &&
          sizeof(M3) == n3 * sizeof(T) && sizeof(M4) == n4 * sizeof(T) && sizeof(M5) == sizeof(std::array<T, 0>)
      );
    }

    {
      using M0 = Mat<T, 2, 3, Types<Types<void, void, void>, Types<void, void, C<0>>>>;
      using M1 = Mat<T, 2, 3, Types<Types<C<1>, void, void>, Types<void, void, C<0>>>>;
      using M2 = Mat<T, 2, 3, Types<Types<C<1>, void, void>, Types<void, C<2>, C<0>>>>;
      using M3 = Mat<T, 2, 3, Types<Types<C<1>, C<3>, void>, Types<void, C<2>, C<0>>>>;
      using M4 = Mat<T, 2, 3, Types<Types<C<1>, C<3>, void>, Types<C<4>, C<2>, C<0>>>>;
      using M5 = Mat<T, 2, 3, Types<Types<C<1>, C<3>, C<5>>, Types<C<4>, C<2>, C<0>>>>;
      constexpr auto i0 = M0::coords2Indices(), i1 = M1::coords2Indices(), i2 = M2::coords2Indices(),
                     i3 = M3::coords2Indices(), i4 = M4::coords2Indices(), i5 = M5::coords2Indices();
      constexpr auto n0 = M0::nValuesToStore(), n1 = M1::nValuesToStore(), n2 = M2::nValuesToStore(),
                     n3 = M3::nValuesToStore(), n4 = M4::nValuesToStore(), n5 = M5::nValuesToStore();
      static_assert(i0[0][0] == 0 & i0[0][1] == 1 & i0[0][2] == 2 & i0[1][0] == 3 & i0[1][1] == 4 & i0[1][2] == -1);
      static_assert(i1[0][0] == -1 & i1[0][1] == 0 & i1[0][2] == 1 & i1[1][0] == 2 & i1[1][1] == 3 & i1[1][2] == -1);
      static_assert(i2[0][0] == -1 & i2[0][1] == 0 & i2[0][2] == 1 & i2[1][0] == 2 & i2[1][1] == -1 & i2[1][2] == -1);
      static_assert(i3[0][0] == -1 & i3[0][1] == -1 & i3[0][2] == 0 & i3[1][0] == 1 & i3[1][1] == -1 & i3[1][2] == -1);
      static_assert(i4[0][0] == -1 & i4[0][1] == -1 & i4[0][2] == 0 & i4[1][0] == -1 & i4[1][1] == -1 & i4[1][2] == -1);
      static_assert(
          i5[0][0] == -1 & i5[0][1] == -1 & i5[0][2] == -1 & i5[1][0] == -1 & i5[1][1] == -1 & i5[1][2] == -1
      );
      static_assert(n0 == 5 && n1 == 4 && n2 == 3 && n3 == 2 && n4 == 1 && n5 == 0);
      static_assert(
          sizeof(M0) == n0 * sizeof(T) && sizeof(M1) == n1 * sizeof(T) && sizeof(M2) == n2 * sizeof(T) &&
          sizeof(M3) == n3 * sizeof(T) && sizeof(M4) == n4 * sizeof(T) && sizeof(M5) == sizeof(std::array<T, 0>)
      );
    }
  };
};

} // namespace
