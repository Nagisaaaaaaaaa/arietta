#include "Arietta/Stateful/Stateful.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

struct A {};

struct B {};

struct C {};

//
//
//
//
//
suite<"Stateful"> _ = [] {
  "Macros"_test = [] {
    ARIETTA_EXPR(0);
    ARIETTA_EXPR(1);
    ARIETTA_EXPR(false);
    ARIETTA_EXPR(true);
    ARIETTA_EXPR(A{});

    ARIETTA_EXPR(0, false);
    ARIETTA_EXPR(1, true);
    ARIETTA_EXPR(A{}, B{});

    ARIETTA_EXPR(0, 1, 2);
    ARIETTA_EXPR(A{}, B{}, C{});
  };
};

} // namespace
