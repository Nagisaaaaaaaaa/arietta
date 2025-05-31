#include "Arietta/Stateful/Index.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

suite<"Index"> _ = [] {
  "Explicit"_test = [] {
    static_assert(Index<>::Load() == 0);
    static_assert(Index<>::Load() == 0);
    static_assert(Index<int>::Load() == 0);
    static_assert(Index<int>::Load() == 0);
    static_assert(Index<float>::Load() == 0);
    static_assert(Index<float>::Load() == 0);
#if 0
    static_assert(Index<0>::Load() == 0);
    static_assert(Index<0>::Load() == 0);
    static_assert(Index<3.14F>::Load() == 0);
    static_assert(Index<3.14F>::Load() == 0);
#endif

    static_assert(Index<>::FetchAdd() == 0);
    static_assert(Index<int>::FetchAdd() == 0);
    static_assert(Index<float>::FetchAdd() == 0);
#if 0
    static_assert(Index<0>::FetchAdd() == 0);
    static_assert(Index<3.14F>::FetchAdd() == 0);
#endif

    static_assert(Index<>::Load() == 1);
    static_assert(Index<>::Load() == 1);
    static_assert(Index<int>::Load() == 1);
    static_assert(Index<int>::Load() == 1);
    static_assert(Index<float>::Load() == 1);
    static_assert(Index<float>::Load() == 1);
#if 0
    static_assert(Index<0>::Load() == 1);
    static_assert(Index<0>::Load() == 1);
    static_assert(Index<3.14F>::Load() == 1);
    static_assert(Index<3.14F>::Load() == 1);
#endif

    static_assert(Index<>::FetchAdd() == 1);
    static_assert(Index<int>::FetchAdd() == 1);
    static_assert(Index<int>::FetchAdd() == 2);
    static_assert(Index<float>::FetchAdd() == 1);
    static_assert(Index<float>::FetchAdd() == 2);
    static_assert(Index<float>::FetchAdd() == 3);
#if 0
    static_assert(Index<0>::FetchAdd() == 1);
    static_assert(Index<0>::FetchAdd() == 2);
    static_assert(Index<3.14F>::FetchAdd() == 1);
#endif

    static_assert(Index<>::Load() == 2);
    static_assert(Index<>::Load() == 2);
    static_assert(Index<int>::Load() == 3);
    static_assert(Index<int>::Load() == 3);
    static_assert(Index<float>::Load() == 4);
    static_assert(Index<float>::Load() == 4);
#if 0
    static_assert(Index<0>::Load() == 3);
    static_assert(Index<0>::Load() == 3);
    static_assert(Index<3.14F>::Load() == 2);
    static_assert(Index<3.14F>::Load() == 2);
#endif

    static_assert(Index<>::FetchAdd() == 2);
    static_assert(Index<int>::FetchAdd() == 3);
    static_assert(Index<float>::FetchAdd() == 4);
#if 0
    static_assert(Index<0>::FetchAdd() == 3);
    static_assert(Index<3.14F>::FetchAdd() == 2);
#endif

    static_assert(Index<>::Load() == 3);
    static_assert(Index<>::Load() == 3);
    static_assert(Index<int>::Load() == 4);
    static_assert(Index<int>::Load() == 4);
    static_assert(Index<float>::Load() == 5);
    static_assert(Index<float>::Load() == 5);
#if 0
    static_assert(Index<0>::Load() == 4);
    static_assert(Index<0>::Load() == 4);
    static_assert(Index<3.14F>::Load() == 3);
    static_assert(Index<3.14F>::Load() == 3);
#endif
  };
};

} // namespace
