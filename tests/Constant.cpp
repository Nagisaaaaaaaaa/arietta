#include "Arietta/Constant.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Constant"> _ = [] {
  "Instantiations"_test = [] {
    static_assert(std::is_same_v<C<0>::type, C<0>>);
    static_assert(std::is_same_v<C<0>::value_type, int>);
    static_assert(C<0>::value == 0);
    {
      C<0> c;
      constexpr int v = c;
      static_assert(v == 0);
    }

    static_assert(std::is_same_v<C<1U>::type, C<1U>>);
    static_assert(std::is_same_v<C<1U>::value_type, unsigned>);
    static_assert(C<1U>::value == 1U);
    {
      C<1U> c;
      constexpr unsigned v = c;
      static_assert(v == 1U);
    }

    static_assert(std::is_same_v<C<2.0F>::type, C<2.0F>>);
    static_assert(std::is_same_v<C<2.0F>::value_type, float>);
    static_assert(C<2.0F>::value == 2.0F);
    {
      C<2.0F> c;
      constexpr float v = c;
      static_assert(v == 2.0F);
    }

    static_assert(std::is_same_v<C<-3.0>::type, C<-3.0>>);
    static_assert(std::is_same_v<C<-3.0>::value_type, double>);
    static_assert(C<-3.0>::value == -3.0);
    {
      C<-3.0> c;
      constexpr double v = c;
      static_assert(v == -3.0);
    }
  };

  //
  //
  //
  "Implicit Casts By Operators"_test = [] {
    static_assert(std::is_same_v<decltype(-3 + C<-3>{}), int>);
    static_assert(std::is_same_v<decltype(0 + C<-3>{}), int>);
    static_assert(std::is_same_v<decltype(3 + C<-3>{}), int>);
    static_assert(std::is_same_v<decltype(-3 + C<0>{}), int>);
    static_assert(std::is_same_v<decltype(0 + C<0>{}), int>);
    static_assert(std::is_same_v<decltype(3 + C<0>{}), int>);
    static_assert(std::is_same_v<decltype(-3 + C<3>{}), int>);
    static_assert(std::is_same_v<decltype(0 + C<3>{}), int>);
    static_assert(std::is_same_v<decltype(3 + C<3>{}), int>);

    static_assert(std::is_same_v<decltype(C<-3>{} + -3), int>);
    static_assert(std::is_same_v<decltype(C<0>{} + -3), int>);
    static_assert(std::is_same_v<decltype(C<3>{} + -3), int>);
    static_assert(std::is_same_v<decltype(C<-3>{} + 0), int>);
    static_assert(std::is_same_v<decltype(C<0>{} + 0), int>);
    static_assert(std::is_same_v<decltype(C<3>{} + 0), int>);
    static_assert(std::is_same_v<decltype(C<-3>{} + 3), int>);
    static_assert(std::is_same_v<decltype(C<0>{} + 3), int>);
    static_assert(std::is_same_v<decltype(C<3>{} + 3), int>);

    expect(-3 + C<-3>{} == -6);
    expect(0 + C<-3>{} == -3);
    expect(3 + C<-3>{} == 0);
    expect(-3 + C<0>{} == -3);
    expect(0 + C<0>{} == 0);
    expect(3 + C<0>{} == 3);
    expect(-3 + C<3>{} == 0);
    expect(0 + C<3>{} == 3);
    expect(3 + C<3>{} == 6);

    expect(C<-3>{} + -3 == -6);
    expect(C<0>{} + -3 == -3);
    expect(C<3>{} + -3 == 0);
    expect(C<-3>{} + 0 == -3);
    expect(C<0>{} + 0 == 0);
    expect(C<3>{} + 0 == 3);
    expect(C<-3>{} + 3 == 0);
    expect(C<0>{} + 3 == 3);
    expect(C<3>{} + 3 == 6);
  };

  //
  //
  //
  "Is And Is Not"_test = [] {
    static_assert(is::C<C<0>>);
    static_assert(is::C<C<1U>>);
    static_assert(is::C<C<2.0F>>);
    static_assert(is::C<C<-3.0>>);

    static_assert(!is::C<int>);
    static_assert(!is::C<unsigned>);
    static_assert(!is::C<float>);
    static_assert(!is::C<double>);

    static_assert(!isnot::C<C<0>>);
    static_assert(!isnot::C<C<1U>>);
    static_assert(!isnot::C<C<2.0F>>);
    static_assert(!isnot::C<C<-3.0>>);

    static_assert(isnot::C<int>);
    static_assert(isnot::C<unsigned>);
    static_assert(isnot::C<float>);
    static_assert(isnot::C<double>);
  };

  //
  //
  //
  "Operator Common Cases"_test = [] {
    static_assert(std::is_same_v<decltype(+C<-3>{}), C<-3>>);
    static_assert(std::is_same_v<decltype(+C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(+C<3>{}), C<3>>);

    static_assert(std::is_same_v<decltype(-C<-3>{}), C<3>>);
    static_assert(std::is_same_v<decltype(-C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(-C<3>{}), C<-3>>);

    static_assert(C<-3>{});
    static_assert(!C<0>{});
    static_assert(C<3>{});
    static_assert(std::is_same_v<decltype(!C<-3>{}), C<false>>);
    static_assert(std::is_same_v<decltype(!C<0>{}), C<true>>);
    static_assert(std::is_same_v<decltype(!C<3>{}), C<false>>);

    static_assert(std::is_same_v<decltype(C<-3>{} + C<-3>{}), C<-6>>);
    static_assert(std::is_same_v<decltype(C<0>{} + C<-3>{}), C<-3>>);
    static_assert(std::is_same_v<decltype(C<3>{} + C<-3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<-3>{} + C<0>{}), C<-3>>);
    static_assert(std::is_same_v<decltype(C<0>{} + C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} + C<0>{}), C<3>>);
    static_assert(std::is_same_v<decltype(C<-3>{} + C<3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<0>{} + C<3>{}), C<3>>);
    static_assert(std::is_same_v<decltype(C<3>{} + C<3>{}), C<6>>);

    static_assert(std::is_same_v<decltype(C<-3>{} - C<-3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<0>{} - C<-3>{}), C<3>>);
    static_assert(std::is_same_v<decltype(C<3>{} - C<-3>{}), C<6>>);
    static_assert(std::is_same_v<decltype(C<-3>{} - C<0>{}), C<-3>>);
    static_assert(std::is_same_v<decltype(C<0>{} - C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} - C<0>{}), C<3>>);
    static_assert(std::is_same_v<decltype(C<-3>{} - C<3>{}), C<-6>>);
    static_assert(std::is_same_v<decltype(C<0>{} - C<3>{}), C<-3>>);
    static_assert(std::is_same_v<decltype(C<3>{} - C<3>{}), C<0>>);

    static_assert(std::is_same_v<decltype(C<-3>{} * C<-3>{}), C<9>>);
    static_assert(std::is_same_v<decltype(C<0>{} * C<-3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} * C<-3>{}), C<-9>>);
    static_assert(std::is_same_v<decltype(C<-3>{} * C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<0>{} * C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} * C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<-3>{} * C<3>{}), C<-9>>);
    static_assert(std::is_same_v<decltype(C<0>{} * C<3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} * C<3>{}), C<9>>);

    static_assert(std::is_same_v<decltype(C<-3>{} / C<-3>{}), C<1>>);
    static_assert(std::is_same_v<decltype(C<0>{} / C<-3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} / C<-3>{}), C<-1>>);
    static_assert(std::is_same_v<decltype(C<-3>{} / C<3>{}), C<-1>>);
    static_assert(std::is_same_v<decltype(C<0>{} / C<3>{}), C<0>>);
    static_assert(std::is_same_v<decltype(C<3>{} / C<3>{}), C<1>>);
  };

  //
  //
  //
  "Operator Special Cases"_test = [] {
    static_assert(std::is_same_v<decltype(C<0>{} * 3), C<0>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} * 3), C<0.0F>>);
    static_assert(std::is_same_v<decltype(C<0>{} * 3.0), C<0.0>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} * 3.0), C<0.0>>);

    static_assert(std::is_same_v<decltype(3 * C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(3.0F * C<0>{}), C<0.0F>>);
    static_assert(std::is_same_v<decltype(3 * C<0.0>{}), C<0.0>>);
    static_assert(std::is_same_v<decltype(3.0F * C<0.0>{}), C<0.0>>);

    static_assert(std::is_same_v<decltype(C<0>{} / 3), C<0>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} / 3), C<0.0F>>);
    static_assert(std::is_same_v<decltype(C<0>{} / 3.0), C<0.0>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} / 3.0), C<0.0>>);

    static_assert(std::is_same_v<decltype(3 % C<1>{}), C<0>>);
    static_assert(std::is_same_v<decltype(3U % C<1>{}), C<0U>>);
    static_assert(std::is_same_v<decltype(3 % C<1U>{}), C<0U>>);
    static_assert(std::is_same_v<decltype(3U % C<1U>{}), C<0U>>);
    static_assert(std::is_same_v<decltype(3 % C<-1>{}), C<0>>);
    static_assert(std::is_same_v<decltype(3LL % C<-1>{}), C<0LL>>);
    static_assert(std::is_same_v<decltype(3 % C<-1LL>{}), C<0LL>>);
    static_assert(std::is_same_v<decltype(3LL % C<-1LL>{}), C<0LL>>);

    static_assert(std::is_same_v<decltype(C<0>{} % 3), C<0>>);
    static_assert(std::is_same_v<decltype(C<0U>{} % 3), C<0U>>);
    static_assert(std::is_same_v<decltype(C<0>{} % 3U), C<0U>>);
    static_assert(std::is_same_v<decltype(C<0U>{} % 3U), C<0U>>);

    static_assert(std::is_same_v<decltype(C<0>{} & 3), C<0>>);
    static_assert(std::is_same_v<decltype(C<0U>{} & 3), C<0U>>);
    static_assert(std::is_same_v<decltype(C<0>{} & 3U), C<0U>>);
    static_assert(std::is_same_v<decltype(C<0U>{} & 3U), C<0U>>);

    static_assert(std::is_same_v<decltype(3 & C<0>{}), C<0>>);
    static_assert(std::is_same_v<decltype(3U & C<0>{}), C<0U>>);
    static_assert(std::is_same_v<decltype(3 & C<0U>{}), C<0U>>);
    static_assert(std::is_same_v<decltype(3U & C<0U>{}), C<0U>>);

    static_assert(std::is_same_v<decltype(C<0>{} && 3), C<false>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} && 3), C<false>>);
    static_assert(std::is_same_v<decltype(C<0>{} && 3.0), C<false>>);
    static_assert(std::is_same_v<decltype(C<0.0F>{} && 3.0), C<false>>);

    static_assert(std::is_same_v<decltype(3 && C<0>{}), C<false>>);
    static_assert(std::is_same_v<decltype(3.0F && C<0>{}), C<false>>);
    static_assert(std::is_same_v<decltype(3 && C<0.0>{}), C<false>>);
    static_assert(std::is_same_v<decltype(3.0F && C<0.0>{}), C<false>>);

    static_assert(std::is_same_v<decltype(C<1>{} || 3), C<true>>);
    static_assert(std::is_same_v<decltype(C<1.0F>{} || 3), C<true>>);
    static_assert(std::is_same_v<decltype(C<1>{} || 3.0), C<true>>);
    static_assert(std::is_same_v<decltype(C<1.0F>{} || 3.0), C<true>>);

    static_assert(std::is_same_v<decltype(3 || C<1>{}), C<true>>);
    static_assert(std::is_same_v<decltype(3.0F || C<1>{}), C<true>>);
    static_assert(std::is_same_v<decltype(3 || C<1.0>{}), C<true>>);
    static_assert(std::is_same_v<decltype(3.0F || C<1.0>{}), C<true>>);
  };
};

} // namespace
