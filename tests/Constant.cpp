#include "Arietta/Constant.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Constant"> _ = [] {
  "Instantiation"_test = [] {
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
};

} // namespace
