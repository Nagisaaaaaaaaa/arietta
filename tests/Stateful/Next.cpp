#include "Arietta/Stateful/Next.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

suite<"Next"> _ = [] {
  "Next"_test = [] {
    static_assert(stateful::Next() == 0);
    static_assert(stateful::Next() == 1);
    static_assert(stateful::Next() == 2);
    static_assert(stateful::Next() == 3);
    static_assert(stateful::Next() == 4);
  };
};

} // namespace
