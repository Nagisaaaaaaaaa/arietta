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
  // static_assert(!is::Same<typename Vector<T>::template Back<>, int>); //! Should not compile.
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::Size() == 1);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template Back<>, int>);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::Size() == 2);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template Back<>, T>);
  SRT_EXPR(Vector<T>::template PushBack<T>());
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::Size() == 3);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template At<2>, T>);
  static_assert(is::Same<typename Vector<T>::template Back<>, T>);
  SRT_EXPR(Vector<T>::template PushBack<int>());
  static_assert(Vector<T>::Size() == 4);
  static_assert(Vector<T>::Size() == 4);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template At<2>, T>);
  static_assert(is::Same<typename Vector<T>::template At<3>, int>);
  static_assert(is::Same<typename Vector<T>::template Back<>, int>);
}

template <typename T>
class ImplicitInstantiationByClass {
  static_assert(Vector<T>::Size() == 0);
  static_assert(Vector<T>::Size() == 0);
  // static_assert(!is::Same<typename Vector<T>::template Back<>, int>); //! Should not compile.
  static_assert(Vector<T>::template PushBack<int>());
  static_assert(Vector<T>::Size() == 1);
  static_assert(Vector<T>::Size() == 1);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template Back<>, int>);
  static_assert(Vector<T>::template PushBack<T>());
  static_assert(Vector<T>::Size() == 2);
  static_assert(Vector<T>::Size() == 2);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template Back<>, T>);
  SRT_EXPR(Vector<T>::template PushBack<T>());
  static_assert(Vector<T>::Size() == 3);
  static_assert(Vector<T>::Size() == 3);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template At<2>, T>);
  static_assert(is::Same<typename Vector<T>::template Back<>, T>);
  SRT_EXPR(Vector<T>::template PushBack<int>());
  static_assert(Vector<T>::Size() == 4);
  static_assert(Vector<T>::Size() == 4);
  static_assert(is::Same<typename Vector<T>::template At<0>, int>);
  static_assert(is::Same<typename Vector<T>::template At<1>, T>);
  static_assert(is::Same<typename Vector<T>::template At<2>, T>);
  static_assert(is::Same<typename Vector<T>::template At<3>, int>);
  static_assert(is::Same<typename Vector<T>::template Back<>, int>);
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
    // static_assert(!is::Same<Vector<>::Back<>, int>);         //! Should not compile.
    // static_assert(!is::Same<Vector<int>::Back<>, unsigned>); //! Should not compile.

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<unsigned>();
    static_assert(Vector<>::Size() == 1);
    static_assert(Vector<>::Size() == 1);
    static_assert(Vector<int>::Size() == 1);
    static_assert(Vector<int>::Size() == 1);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::Back<>, int>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::Back<>, unsigned>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(Vector<>::Size() == 2);
    static_assert(Vector<>::Size() == 2);
    static_assert(Vector<int>::Size() == 2);
    static_assert(Vector<int>::Size() == 2);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::Back<>, float>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::Back<>, double>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<double>();
    static_assert(Vector<>::Size() == 3);
    static_assert(Vector<>::Size() == 3);
    static_assert(Vector<int>::Size() == 3);
    static_assert(Vector<int>::Size() == 3);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::Back<>, float>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::Back<>, double>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string_view>();
    static_assert(Vector<>::Size() == 4);
    static_assert(Vector<>::Size() == 4);
    static_assert(Vector<int>::Size() == 4);
    static_assert(Vector<int>::Size() == 4);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::Back<>, std::string>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::Back<>, std::string_view>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string_view>();
    static_assert(Vector<>::Size() == 5);
    static_assert(Vector<>::Size() == 5);
    static_assert(Vector<int>::Size() == 5);
    static_assert(Vector<int>::Size() == 5);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::At<4>, std::string>);
    static_assert(is::Same<Vector<>::Back<>, std::string>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<4>, std::string_view>);
    static_assert(is::Same<Vector<int>::Back<>, std::string_view>);

    Vector<>::PushBack<std::string>();
    Vector<int>::PushBack<std::string>();
    static_assert(Vector<>::Size() == 6);
    static_assert(Vector<>::Size() == 6);
    static_assert(Vector<int>::Size() == 6);
    static_assert(Vector<int>::Size() == 6);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::At<4>, std::string>);
    static_assert(is::Same<Vector<>::At<5>, std::string>);
    static_assert(is::Same<Vector<>::Back<>, std::string>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<4>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<5>, std::string>);
    static_assert(is::Same<Vector<int>::Back<>, std::string>);

    Vector<>::PushBack<float>();
    Vector<int>::PushBack<float>();
    static_assert(Vector<>::Size() == 7);
    static_assert(Vector<>::Size() == 7);
    static_assert(Vector<int>::Size() == 7);
    static_assert(Vector<int>::Size() == 7);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::At<4>, std::string>);
    static_assert(is::Same<Vector<>::At<5>, std::string>);
    static_assert(is::Same<Vector<>::At<6>, float>);
    static_assert(is::Same<Vector<>::Back<>, float>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<4>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<5>, std::string>);
    static_assert(is::Same<Vector<int>::At<6>, float>);
    static_assert(is::Same<Vector<int>::Back<>, float>);

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<unsigned>();
    static_assert(Vector<>::Size() == 8);
    static_assert(Vector<>::Size() == 8);
    static_assert(Vector<int>::Size() == 8);
    static_assert(Vector<int>::Size() == 8);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::At<4>, std::string>);
    static_assert(is::Same<Vector<>::At<5>, std::string>);
    static_assert(is::Same<Vector<>::At<6>, float>);
    static_assert(is::Same<Vector<>::At<7>, int>);
    static_assert(is::Same<Vector<>::Back<>, int>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<4>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<5>, std::string>);
    static_assert(is::Same<Vector<int>::At<6>, float>);
    static_assert(is::Same<Vector<int>::At<7>, unsigned>);
    static_assert(is::Same<Vector<int>::Back<>, unsigned>);

    Vector<>::PushBack<int>();
    Vector<int>::PushBack<int>();
    static_assert(Vector<>::Size() == 9);
    static_assert(Vector<>::Size() == 9);
    static_assert(Vector<int>::Size() == 9);
    static_assert(Vector<int>::Size() == 9);
    static_assert(is::Same<Vector<>::At<0>, int>);
    static_assert(is::Same<Vector<>::At<1>, float>);
    static_assert(is::Same<Vector<>::At<2>, float>);
    static_assert(is::Same<Vector<>::At<3>, std::string>);
    static_assert(is::Same<Vector<>::At<4>, std::string>);
    static_assert(is::Same<Vector<>::At<5>, std::string>);
    static_assert(is::Same<Vector<>::At<6>, float>);
    static_assert(is::Same<Vector<>::At<7>, int>);
    static_assert(is::Same<Vector<>::At<8>, int>);
    static_assert(is::Same<Vector<>::Back<>, int>);
    static_assert(is::Same<Vector<int>::At<0>, unsigned>);
    static_assert(is::Same<Vector<int>::At<1>, double>);
    static_assert(is::Same<Vector<int>::At<2>, double>);
    static_assert(is::Same<Vector<int>::At<3>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<4>, std::string_view>);
    static_assert(is::Same<Vector<int>::At<5>, std::string>);
    static_assert(is::Same<Vector<int>::At<6>, float>);
    static_assert(is::Same<Vector<int>::At<7>, unsigned>);
    static_assert(is::Same<Vector<int>::At<8>, int>);
    static_assert(is::Same<Vector<int>::Back<>, int>);
  };

  //
  //
  //
  "Implicit"_test = [] {
    auto implicitInstantiationByLambda = []<typename T> {
      static_assert(Vector<T>::Size() == 0);
      static_assert(Vector<T>::Size() == 0);
      // static_assert(!is::Same<typename Vector<T>::template Back<>, int>); //! Should not compile.
      static_assert(Vector<T>::template PushBack<int>());
      static_assert(Vector<T>::Size() == 1);
      static_assert(Vector<T>::Size() == 1);
      static_assert(is::Same<typename Vector<T>::template At<0>, int>);
      static_assert(is::Same<typename Vector<T>::template Back<>, int>);
      static_assert(Vector<T>::template PushBack<T>());
      static_assert(Vector<T>::Size() == 2);
      static_assert(Vector<T>::Size() == 2);
      static_assert(is::Same<typename Vector<T>::template At<0>, int>);
      static_assert(is::Same<typename Vector<T>::template At<1>, T>);
      static_assert(is::Same<typename Vector<T>::template Back<>, T>);
      SRT_EXPR(Vector<T>::template PushBack<T>());
      static_assert(Vector<T>::Size() == 3);
      static_assert(Vector<T>::Size() == 3);
      static_assert(is::Same<typename Vector<T>::template At<0>, int>);
      static_assert(is::Same<typename Vector<T>::template At<1>, T>);
      static_assert(is::Same<typename Vector<T>::template At<2>, T>);
      static_assert(is::Same<typename Vector<T>::template Back<>, T>);
      SRT_EXPR(Vector<T>::template PushBack<int>());
      static_assert(Vector<T>::Size() == 4);
      static_assert(Vector<T>::Size() == 4);
      static_assert(is::Same<typename Vector<T>::template At<0>, int>);
      static_assert(is::Same<typename Vector<T>::template At<1>, T>);
      static_assert(is::Same<typename Vector<T>::template At<2>, T>);
      static_assert(is::Same<typename Vector<T>::template At<3>, int>);
      static_assert(is::Same<typename Vector<T>::template Back<>, int>);
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
