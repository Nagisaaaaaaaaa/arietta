#include "Arietta/Stateful/Vector.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

suite<"Vector"> _ = [] {
  "Explicit"_test = [] {
    Vector<>::PushBack<int>();
    Vector<int>::PushBack<unsigned>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string_view>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string_view>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<4>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<4>, std::string_view>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<4>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<4>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<5>, std::string>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<float>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<4>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<6>, float>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<4>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<6>, float>);

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<unsigned>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<4>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<6>, float>);
    static_assert(std::is_same_v<Vector<>::At<7>, int>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<4>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<6>, float>);
    static_assert(std::is_same_v<Vector<int>::At<7>, unsigned>);

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<int>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<>::At<3>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<4>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<>::At<6>, float>);
    static_assert(std::is_same_v<Vector<>::At<7>, int>);
    static_assert(std::is_same_v<Vector<>::At<8>, int>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(std::is_same_v<Vector<int>::At<3>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<4>, std::string_view>);
    static_assert(std::is_same_v<Vector<int>::At<5>, std::string>);
    static_assert(std::is_same_v<Vector<int>::At<6>, float>);
    static_assert(std::is_same_v<Vector<int>::At<7>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<8>, int>);
  };
};

} // namespace
