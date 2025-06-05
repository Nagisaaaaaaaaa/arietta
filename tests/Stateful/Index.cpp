#include "Arietta/Stateful/Index.hpp"

#include <boost/ut.hpp>

using namespace arietta;
using namespace arietta::stateful;
using namespace boost::ut;

namespace {

template <typename T>
void ImplicitInstantiationByFunction() {
  static_assert(Index<T>::Load() == 0);
  static_assert(Index<T>::Load() == 0);
  static_assert(Index<T>::FetchAdd() == 0);
  static_assert(Index<T>::Load() == 1);
  static_assert(Index<T>::Load() == 1);
  SRT_EXPR(Index<T>::FetchAdd());
  SRT_EXPR(Index<T>::FetchAdd());
  static_assert(Index<T>::Load() == 3);
  static_assert(Index<T>::Load() == 3);
  static_assert(Index<T>::FetchAdd() == 3);
  static_assert(Index<T>::Load() == 4);
  static_assert(Index<T>::Load() == 4);
};

template <typename T>
class ImplicitInstantiationByClass {
  static_assert(Index<T>::Load() == 0);
  static_assert(Index<T>::Load() == 0);
  static_assert(Index<T>::FetchAdd() == 0);
  static_assert(Index<T>::Load() == 1);
  static_assert(Index<T>::Load() == 1);
  SRT_EXPR(Index<T>::FetchAdd());
  SRT_EXPR(Index<T>::FetchAdd());
  static_assert(Index<T>::Load() == 3);
  static_assert(Index<T>::Load() == 3);
  static_assert(Index<T>::FetchAdd() == 3);
  static_assert(Index<T>::Load() == 4);
  static_assert(Index<T>::Load() == 4);
};

//
//
//
//
//
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

    Index<>::FetchAdd();
    Index<int>::FetchAdd();
    Index<int>::FetchAdd();
    Index<float>::FetchAdd();
    Index<float>::FetchAdd();
    Index<float>::FetchAdd();
#if 0
    Index<0>::FetchAdd();
    Index<0>::FetchAdd();
    Index<3.14F>::FetchAdd();
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

  //
  //
  //
  "Implicit"_test = [] {
    auto implicitInstantiationByLambda = []<typename T> {
      static_assert(Index<T>::Load() == 0);
      static_assert(Index<T>::Load() == 0);
      static_assert(Index<T>::FetchAdd() == 0);
      static_assert(Index<T>::Load() == 1);
      static_assert(Index<T>::Load() == 1);
      SRT_EXPR(Index<T>::FetchAdd());
      SRT_EXPR(Index<T>::FetchAdd());
      static_assert(Index<T>::Load() == 3);
      static_assert(Index<T>::Load() == 3);
      static_assert(Index<T>::FetchAdd() == 3);
      static_assert(Index<T>::Load() == 4);
      static_assert(Index<T>::Load() == 4);
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
