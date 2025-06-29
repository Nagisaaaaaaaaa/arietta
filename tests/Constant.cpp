#include "Arietta/Constant.hpp"

#include <boost/ut.hpp>
#include <cmath>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Constant"> _ = [] {
  "Instantiations"_test = [] {
    static_assert(is::Same<C<0>::type, C<0>>);
    static_assert(is::Same<C<0>::value_type, int>);
    static_assert(C<0>::value == 0);
    {
      C<0> c;
      constexpr int v = c;
      static_assert(v == 0);
    }

    static_assert(is::Same<C<1U>::type, C<1U>>);
    static_assert(is::Same<C<1U>::value_type, unsigned>);
    static_assert(C<1U>::value == 1U);
    {
      C<1U> c;
      constexpr unsigned v = c;
      static_assert(v == 1U);
    }

    static_assert(is::Same<C<2.0F>::type, C<2.0F>>);
    static_assert(is::Same<C<2.0F>::value_type, float>);
    static_assert(C<2.0F>::value == 2.0F);
    {
      C<2.0F> c;
      constexpr float v = c;
      static_assert(v == 2.0F);
    }

    static_assert(is::Same<C<-3.0>::type, C<-3.0>>);
    static_assert(is::Same<C<-3.0>::value_type, double>);
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
    static_assert(is::Same<decltype(-3 + C<-3>{}), int>);
    static_assert(is::Same<decltype(0 + C<-3>{}), int>);
    static_assert(is::Same<decltype(3 + C<-3>{}), int>);
    static_assert(is::Same<decltype(-3 + C<0>{}), int>);
    static_assert(is::Same<decltype(0 + C<0>{}), int>);
    static_assert(is::Same<decltype(3 + C<0>{}), int>);
    static_assert(is::Same<decltype(-3 + C<3>{}), int>);
    static_assert(is::Same<decltype(0 + C<3>{}), int>);
    static_assert(is::Same<decltype(3 + C<3>{}), int>);

    static_assert(is::Same<decltype(C<-3>{} + -3), int>);
    static_assert(is::Same<decltype(C<0>{} + -3), int>);
    static_assert(is::Same<decltype(C<3>{} + -3), int>);
    static_assert(is::Same<decltype(C<-3>{} + 0), int>);
    static_assert(is::Same<decltype(C<0>{} + 0), int>);
    static_assert(is::Same<decltype(C<3>{} + 0), int>);
    static_assert(is::Same<decltype(C<-3>{} + 3), int>);
    static_assert(is::Same<decltype(C<0>{} + 3), int>);
    static_assert(is::Same<decltype(C<3>{} + 3), int>);

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
    static_assert(is::Same<decltype(+C<-3>{}), C<-3>>);
    static_assert(is::Same<decltype(+C<0>{}), C<0>>);
    static_assert(is::Same<decltype(+C<3>{}), C<3>>);

    static_assert(is::Same<decltype(-C<-3>{}), C<3>>);
    static_assert(is::Same<decltype(-C<0>{}), C<0>>);
    static_assert(is::Same<decltype(-C<3>{}), C<-3>>);

    static_assert(C<-3>{});
    static_assert(!C<0>{});
    static_assert(C<3>{});
    static_assert(is::Same<decltype(!C<-3>{}), C<false>>);
    static_assert(is::Same<decltype(!C<0>{}), C<true>>);
    static_assert(is::Same<decltype(!C<3>{}), C<false>>);

    static_assert(is::Same<decltype(C<-3>{} + C<-3>{}), C<-6>>);
    static_assert(is::Same<decltype(C<0>{} + C<-3>{}), C<-3>>);
    static_assert(is::Same<decltype(C<3>{} + C<-3>{}), C<0>>);
    static_assert(is::Same<decltype(C<-3>{} + C<0>{}), C<-3>>);
    static_assert(is::Same<decltype(C<0>{} + C<0>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} + C<0>{}), C<3>>);
    static_assert(is::Same<decltype(C<-3>{} + C<3>{}), C<0>>);
    static_assert(is::Same<decltype(C<0>{} + C<3>{}), C<3>>);
    static_assert(is::Same<decltype(C<3>{} + C<3>{}), C<6>>);

    static_assert(is::Same<decltype(C<-3>{} - C<-3>{}), C<0>>);
    static_assert(is::Same<decltype(C<0>{} - C<-3>{}), C<3>>);
    static_assert(is::Same<decltype(C<3>{} - C<-3>{}), C<6>>);
    static_assert(is::Same<decltype(C<-3>{} - C<0>{}), C<-3>>);
    static_assert(is::Same<decltype(C<0>{} - C<0>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} - C<0>{}), C<3>>);
    static_assert(is::Same<decltype(C<-3>{} - C<3>{}), C<-6>>);
    static_assert(is::Same<decltype(C<0>{} - C<3>{}), C<-3>>);
    static_assert(is::Same<decltype(C<3>{} - C<3>{}), C<0>>);

    static_assert(is::Same<decltype(C<-3>{} * C<-3>{}), C<9>>);
    static_assert(is::Same<decltype(C<0>{} * C<-3>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} * C<-3>{}), C<-9>>);
    static_assert(is::Same<decltype(C<-3>{} * C<0>{}), C<0>>);
    static_assert(is::Same<decltype(C<0>{} * C<0>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} * C<0>{}), C<0>>);
    static_assert(is::Same<decltype(C<-3>{} * C<3>{}), C<-9>>);
    static_assert(is::Same<decltype(C<0>{} * C<3>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} * C<3>{}), C<9>>);

    static_assert(is::Same<decltype(C<-3>{} / C<-3>{}), C<1>>);
    static_assert(is::Same<decltype(C<0>{} / C<-3>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} / C<-3>{}), C<-1>>);
    static_assert(is::Same<decltype(C<-3>{} / C<3>{}), C<-1>>);
    static_assert(is::Same<decltype(C<0>{} / C<3>{}), C<0>>);
    static_assert(is::Same<decltype(C<3>{} / C<3>{}), C<1>>);
  };

  //
  //
  //
  "Operator Special Cases"_test = [] {
    static_assert(is::Same<decltype(C<0>{} * 3), C<0>>);
    static_assert(is::Same<decltype(C<0.0F>{} * 3), C<0.0F>>);
    static_assert(is::Same<decltype(C<0>{} * 3.0), C<0.0>>);
    static_assert(is::Same<decltype(C<0.0F>{} * 3.0), C<0.0>>);

    static_assert(is::Same<decltype(3 * C<0>{}), C<0>>);
    static_assert(is::Same<decltype(3.0F * C<0>{}), C<0.0F>>);
    static_assert(is::Same<decltype(3 * C<0.0>{}), C<0.0>>);
    static_assert(is::Same<decltype(3.0F * C<0.0>{}), C<0.0>>);

    static_assert(is::Same<decltype(C<0>{} / 3), C<0>>);
    static_assert(is::Same<decltype(C<0.0F>{} / 3), C<0.0F>>);
    static_assert(is::Same<decltype(C<0>{} / 3.0), C<0.0>>);
    static_assert(is::Same<decltype(C<0.0F>{} / 3.0), C<0.0>>);

    static_assert(is::Same<decltype(3 % C<1>{}), C<0>>);
    static_assert(is::Same<decltype(3U % C<1>{}), C<0U>>);
    static_assert(is::Same<decltype(3 % C<1U>{}), C<0U>>);
    static_assert(is::Same<decltype(3U % C<1U>{}), C<0U>>);
    static_assert(is::Same<decltype(3 % C<-1>{}), C<0>>);
    static_assert(is::Same<decltype(3LL % C<-1>{}), C<0LL>>);
    static_assert(is::Same<decltype(3 % C<-1LL>{}), C<0LL>>);
    static_assert(is::Same<decltype(3LL % C<-1LL>{}), C<0LL>>);

    static_assert(is::Same<decltype(C<0>{} % 3), C<0>>);
    static_assert(is::Same<decltype(C<0U>{} % 3), C<0U>>);
    static_assert(is::Same<decltype(C<0>{} % 3U), C<0U>>);
    static_assert(is::Same<decltype(C<0U>{} % 3U), C<0U>>);

    static_assert(is::Same<decltype(C<0>{} & 3), C<0>>);
    static_assert(is::Same<decltype(C<0U>{} & 3), C<0U>>);
    static_assert(is::Same<decltype(C<0>{} & 3U), C<0U>>);
    static_assert(is::Same<decltype(C<0U>{} & 3U), C<0U>>);

    static_assert(is::Same<decltype(3 & C<0>{}), C<0>>);
    static_assert(is::Same<decltype(3U & C<0>{}), C<0U>>);
    static_assert(is::Same<decltype(3 & C<0U>{}), C<0U>>);
    static_assert(is::Same<decltype(3U & C<0U>{}), C<0U>>);

    static_assert(is::Same<decltype(C<0>{} && 3), C<false>>);
    static_assert(is::Same<decltype(C<0.0F>{} && 3), C<false>>);
    static_assert(is::Same<decltype(C<0>{} && 3.0), C<false>>);
    static_assert(is::Same<decltype(C<0.0F>{} && 3.0), C<false>>);

    static_assert(is::Same<decltype(3 && C<0>{}), C<false>>);
    static_assert(is::Same<decltype(3.0F && C<0>{}), C<false>>);
    static_assert(is::Same<decltype(3 && C<0.0>{}), C<false>>);
    static_assert(is::Same<decltype(3.0F && C<0.0>{}), C<false>>);

    static_assert(is::Same<decltype(C<1>{} || 3), C<true>>);
    static_assert(is::Same<decltype(C<1.0F>{} || 3), C<true>>);
    static_assert(is::Same<decltype(C<1>{} || 3.0), C<true>>);
    static_assert(is::Same<decltype(C<1.0F>{} || 3.0), C<true>>);

    static_assert(is::Same<decltype(3 || C<1>{}), C<true>>);
    static_assert(is::Same<decltype(3.0F || C<1>{}), C<true>>);
    static_assert(is::Same<decltype(3 || C<1.0>{}), C<true>>);
    static_assert(is::Same<decltype(3.0F || C<1.0>{}), C<true>>);
  };

  //
  //
  //
  "For Each"_test = [] {
    // `auto` type deductions.
    {
      auto f0 = [](is::C auto i) { static_assert(i != i); };
      auto f1 = []<auto i> { static_assert(i != i); };
      ForEach(C<0>{}, f0), ForEach<0>(f0), ForEach<C<0>>(f0);
      ForEach(C<0>{}, f1), ForEach<0>(f1), ForEach<C<0>>(f1);
    }
    {
      std::string s0, s1;
      auto f0 = [&](is::C auto i) {
        static_assert(is::Same<typename decltype(i)::value_type, int>);
        s0 += std::to_string(static_cast<int>(i));
      };
      auto f1 = [&]<auto i> {
        static_assert(is::Same<decltype(i), int>);
        s1 += std::to_string(static_cast<int>(i));
      };
      ForEach(C<1>{}, f0), ForEach<1>(f0), ForEach<C<1>>(f0);
      ForEach(C<1>{}, f1), ForEach<1>(f1), ForEach<C<1>>(f1);
      expect(s0 == "000" && s1 == "000");
    }
    {
      std::string s0, s1;
      auto f0 = [&](is::C auto i) {
        static_assert(is::Same<typename decltype(i)::value_type, unsigned>);
        s0 += std::to_string(static_cast<int>(i));
      };
      auto f1 = [&]<auto i> {
        static_assert(is::Same<decltype(i), unsigned>);
        s1 += std::to_string(static_cast<int>(i));
      };
      ForEach(C<2U>{}, f0), ForEach<2U>(f0), ForEach<C<2U>>(f0);
      ForEach(C<2U>{}, f1), ForEach<2U>(f1), ForEach<C<2U>>(f1);
      expect(s0 == "010101" && s1 == "010101");
    }
    {
      std::string s0, s1;
      auto f0 = [&](is::C auto i) {
        static_assert(is::Same<typename decltype(i)::value_type, float>);
        s0 += std::to_string(static_cast<int>(i));
      };
      auto f1 = [&]<auto i> {
        static_assert(is::Same<decltype(i), float>);
        s1 += std::to_string(static_cast<int>(i));
      };
      ForEach(C<3.0F>{}, f0), ForEach<3.0F>(f0), ForEach<C<3.0F>>(f0);
      ForEach(C<3.0F>{}, f1), ForEach<3.0F>(f1), ForEach<C<3.0F>>(f1);
      expect(s0 == "012012012" && s1 == "012012012");
    }
    {
      std::string s0, s1;
      auto f0 = [&](is::C auto i) {
        static_assert(is::Same<typename decltype(i)::value_type, double>);
        s0 += std::to_string(static_cast<int>(i));
      };
      auto f1 = [&]<auto i> {
        static_assert(is::Same<decltype(i), double>);
        s1 += std::to_string(static_cast<int>(i));
      };
      ForEach(C<4.0>{}, f0), ForEach<4.0>(f0), ForEach<C<4.0>>(f0);
      ForEach(C<4.0>{}, f1), ForEach<4.0>(f1), ForEach<C<4.0>>(f1);
      expect(s0 == "012301230123" && s1 == "012301230123");
    }

    // `constexpr`.
    static_assert([] {
      int v{};
      ForEach(C<5U>{}, [&](is::C auto i) { v += i; });
      ForEach(C<5U>{}, [&]<auto i> { v += i; });
      ForEach<6.0F>([&](is::C auto i) { v += i; });
      ForEach<6.0F>([&]<auto i> { v += i; });
      ForEach<C<7.0>>([&](is::C auto i) { v += i; });
      ForEach<C<7.0>>([&]<auto i> { v += i; });
      return v;
    }() == (0 + 1 + 2 + 3 + 4) * 2 + (0 + 1 + 2 + 3 + 4 + 5) * 2 + (0 + 1 + 2 + 3 + 4 + 5 + 6) * 2);

    // Floating point round-off errors.
    auto f0 = [](is::C auto i) {
      expect(i == std::round(decltype(i)::value));
      expect(i == static_cast<int>(i));
    };
    auto f1 = []<auto i> {
      expect(i == std::round(i));
      expect(i == static_cast<int>(i));
    };
    ForEach(C<100.0F>{}, f0), ForEach<100.0F>(f0), ForEach<C<100.0F>>(f0);
    ForEach(C<100.0F>{}, f1), ForEach<100.0F>(f1), ForEach<C<100.0F>>(f1);
    ForEach(C<100.0>{}, f0), ForEach<100.0>(f0), ForEach<C<100.0>>(f0);
    ForEach(C<100.0>{}, f1), ForEach<100.0>(f1), ForEach<C<100.0>>(f1);
  };
};

} // namespace
