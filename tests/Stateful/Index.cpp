#include "Arietta/Stateful/Index.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

suite<"Index"> _ = [] {
  "Index"_test = [] {
    static_assert(Load() == 0);
    static_assert(Load() == 0);
    static_assert(Load<int>() == 0);
    static_assert(Load<int>() == 0);
    static_assert(Load<float>() == 0);
    static_assert(Load<float>() == 0);
    static_assert(Load<0>() == 0);
    static_assert(Load<0>() == 0);
    static_assert(Load<3.14F>() == 0);
    static_assert(Load<3.14F>() == 0);

    static_assert(FetchAdd() == 0);
    static_assert(FetchAdd<int>() == 0);
    static_assert(FetchAdd<float>() == 0);
    static_assert(FetchAdd<0>() == 0);
    static_assert(FetchAdd<3.14F>() == 0);

    static_assert(Load() == 1);
    static_assert(Load() == 1);
    static_assert(Load<int>() == 1);
    static_assert(Load<int>() == 1);
    static_assert(Load<float>() == 1);
    static_assert(Load<float>() == 1);
    static_assert(Load<0>() == 1);
    static_assert(Load<0>() == 1);
    static_assert(Load<3.14F>() == 1);
    static_assert(Load<3.14F>() == 1);

    static_assert(FetchAdd() == 1);
    static_assert(FetchAdd<int>() == 1);
    static_assert(FetchAdd<int>() == 2);
    static_assert(FetchAdd<float>() == 1);
    static_assert(FetchAdd<float>() == 2);
    static_assert(FetchAdd<float>() == 3);
    static_assert(FetchAdd<0>() == 1);
    static_assert(FetchAdd<0>() == 2);
    static_assert(FetchAdd<3.14F>() == 1);

    static_assert(Load() == 2);
    static_assert(Load() == 2);
    static_assert(Load<int>() == 3);
    static_assert(Load<int>() == 3);
    static_assert(Load<float>() == 4);
    static_assert(Load<float>() == 4);
    static_assert(Load<0>() == 3);
    static_assert(Load<0>() == 3);
    static_assert(Load<3.14F>() == 2);
    static_assert(Load<3.14F>() == 2);

    static_assert(FetchAdd() == 2);
    static_assert(FetchAdd<int>() == 3);
    static_assert(FetchAdd<float>() == 4);
    static_assert(FetchAdd<0>() == 3);
    static_assert(FetchAdd<3.14F>() == 2);

    static_assert(Load() == 3);
    static_assert(Load() == 3);
    static_assert(Load<int>() == 4);
    static_assert(Load<int>() == 4);
    static_assert(Load<float>() == 5);
    static_assert(Load<float>() == 5);
    static_assert(Load<0>() == 4);
    static_assert(Load<0>() == 4);
    static_assert(Load<3.14F>() == 3);
    static_assert(Load<3.14F>() == 3);
  };
};

} // namespace
