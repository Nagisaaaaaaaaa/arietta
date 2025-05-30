#include "Arietta/Stateful/Next.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Next"> _ = [] {
  "Next"_test = [] {
    static_assert(stateful::Next() == 0);
    static_assert(stateful::Next<int>() == 0);
    static_assert(stateful::Next<float>() == 0);
    static_assert(stateful::Next<0>() == 0);
    static_assert(stateful::Next<3.14F>() == 0);

    static_assert(stateful::Next() == 1);
    static_assert(stateful::Next<int>() == 1);
    static_assert(stateful::Next<int>() == 2);
    static_assert(stateful::Next<float>() == 1);
    static_assert(stateful::Next<float>() == 2);
    static_assert(stateful::Next<float>() == 3);
    static_assert(stateful::Next<0>() == 1);
    static_assert(stateful::Next<0>() == 2);
    static_assert(stateful::Next<3.14F>() == 1);

    static_assert(stateful::Next() == 2);
    static_assert(stateful::Next<int>() == 3);
    static_assert(stateful::Next<float>() == 4);
    static_assert(stateful::Next<0>() == 3);
    static_assert(stateful::Next<3.14F>() == 2);
  };
};

} // namespace
