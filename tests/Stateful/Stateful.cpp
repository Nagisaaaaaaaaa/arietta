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
    SRT_EXPR(0);
    SRT_EXPR(1);
    SRT_EXPR(false);
    SRT_EXPR(true);
    SRT_EXPR(A{});

    SRT_EXPR(0, false);
    SRT_EXPR(1, true);
    SRT_EXPR(A{}, B{});

    SRT_EXPR(0, 1, 2);
    SRT_EXPR(A{}, B{}, C{});
  };
};

} // namespace
