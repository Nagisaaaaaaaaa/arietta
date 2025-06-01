#include "Arietta/Stateful/Vector.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

template <typename T>
void ImplicitInstantiationByFunction() {
  static_assert(Vector<T>::Size() == 0);
  static_assert(Vector<T>::Size() == 0);
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<3>, int>);
  static_assert(Vector<T>::Size() == 4);
  static_assert(Vector<T>::Size() == 4);
}

template <typename T>
class ImplicitInstantiationByClass {
  static_assert(Vector<T>::Size() == 0);
  static_assert(Vector<T>::Size() == 0);
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
  static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
  static_assert(std::is_same_v<typename Vector<T>::template At<3>, int>);
  static_assert(Vector<T>::Size() == 4);
  static_assert(Vector<T>::Size() == 4);
};

//
//
//
//
//
suite<"Vector"> _ = [] {
  "Explicit"_test = [] {
    static_assert(Vector<>::Size() == 0);
    static_assert(Vector<>::Size() == 0);
    static_assert(Vector<int>::Size() == 0);
    static_assert(Vector<int>::Size() == 0);

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<unsigned>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(Vector<>::Size() == 1);
    static_assert(Vector<>::Size() == 1);
    static_assert(Vector<int>::Size() == 1);
    static_assert(Vector<int>::Size() == 1);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(Vector<>::Size() == 2);
    static_assert(Vector<>::Size() == 2);
    static_assert(Vector<int>::Size() == 2);
    static_assert(Vector<int>::Size() == 2);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(std::is_same_v<Vector<>::At<0>, int>);
    static_assert(std::is_same_v<Vector<>::At<1>, float>);
    static_assert(std::is_same_v<Vector<>::At<2>, float>);
    static_assert(std::is_same_v<Vector<int>::At<0>, unsigned>);
    static_assert(std::is_same_v<Vector<int>::At<1>, double>);
    static_assert(std::is_same_v<Vector<int>::At<2>, double>);
    static_assert(Vector<>::Size() == 3);
    static_assert(Vector<>::Size() == 3);
    static_assert(Vector<int>::Size() == 3);
    static_assert(Vector<int>::Size() == 3);

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
    static_assert(Vector<>::Size() == 4);
    static_assert(Vector<>::Size() == 4);
    static_assert(Vector<int>::Size() == 4);
    static_assert(Vector<int>::Size() == 4);

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
    static_assert(Vector<>::Size() == 5);
    static_assert(Vector<>::Size() == 5);
    static_assert(Vector<int>::Size() == 5);
    static_assert(Vector<int>::Size() == 5);

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
    static_assert(Vector<>::Size() == 6);
    static_assert(Vector<>::Size() == 6);
    static_assert(Vector<int>::Size() == 6);
    static_assert(Vector<int>::Size() == 6);

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
    static_assert(Vector<>::Size() == 7);
    static_assert(Vector<>::Size() == 7);
    static_assert(Vector<int>::Size() == 7);
    static_assert(Vector<int>::Size() == 7);

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
    static_assert(Vector<>::Size() == 8);
    static_assert(Vector<>::Size() == 8);
    static_assert(Vector<int>::Size() == 8);
    static_assert(Vector<int>::Size() == 8);

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
    static_assert(Vector<>::Size() == 9);
    static_assert(Vector<>::Size() == 9);
    static_assert(Vector<int>::Size() == 9);
    static_assert(Vector<int>::Size() == 9);
  };

  //
  //
  //
  "Implicit"_test = [] {
    auto implicitInstantiationByLambda = []<typename T> {
      static_assert(Vector<T>::Size() == 0);
      static_assert(Vector<T>::Size() == 0);
      static_assert(Vector<T>::template PushBack<int>());
      static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
      static_assert(Vector<T>::Size() == 1);
      static_assert(Vector<T>::Size() == 1);
      static_assert(Vector<T>::template PushBack<T>());
      static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
      static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
      static_assert(Vector<T>::Size() == 2);
      static_assert(Vector<T>::Size() == 2);
      static_assert(Vector<T>::template PushBack<T>());
      static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
      static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
      static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
      static_assert(Vector<T>::Size() == 3);
      static_assert(Vector<T>::Size() == 3);
      static_assert(Vector<T>::template PushBack<int>());
      static_assert(std::is_same_v<typename Vector<T>::template At<0>, int>);
      static_assert(std::is_same_v<typename Vector<T>::template At<1>, T>);
      static_assert(std::is_same_v<typename Vector<T>::template At<2>, T>);
      static_assert(std::is_same_v<typename Vector<T>::template At<3>, int>);
      static_assert(Vector<T>::Size() == 4);
      static_assert(Vector<T>::Size() == 4);
    };

    ImplicitInstantiationByFunction<u8>();
    ImplicitInstantiationByFunction<u8 const>();
    ImplicitInstantiationByFunction<u8 volatile>();
    ImplicitInstantiationByClass<u16>{};
    ImplicitInstantiationByClass<u16 const>{};
    ImplicitInstantiationByClass<u16 volatile>{};
    implicitInstantiationByLambda.operator()<u32>();
    implicitInstantiationByLambda.operator()<u32 const>();
    implicitInstantiationByLambda.operator()<u32 volatile>();
  };
};

} // namespace
