#include "Arietta/Types.hpp"

#include <array>
#include <boost/ut.hpp>

using namespace arietta;
using namespace boost::ut;

namespace {

using Tss = Types<void, Types<void>, Types<void, Types<void>>>;

using Ts0 = Types<>;
using Ts1 = Types<i8>;
using Ts2 = Types<i8, i16>;
using Ts3 = Types<i8, i16, Tss>;
using Ts4 = Types<i8, i16, Tss, i16>;
using Ts5 = Types<i8, i16, Tss, i16, Tss>;
using Ts6 = Types<i8, i16, Tss, i16, Tss, Tss>;

template <usize>
using FillEachCase0 = void;

template <usize i>
using FillEachCase1 = C<static_cast<int>(i)>;

template <usize i>
using FillEachCase2 = std::conditional_t<i == 0, i8, std::conditional_t<i == 1 || i == 3, i16, Tss>>;

//
//
//
//
//
suite<"Types"> _ = [] {
  "Size"_test = [] {
    static_assert(Ts0::Size() == 0);
    static_assert(Ts1::Size() == 1);
    static_assert(Ts2::Size() == 2);
    static_assert(Ts3::Size() == 3);
    static_assert(Ts4::Size() == 4);
    static_assert(Ts5::Size() == 5);
    static_assert(Ts6::Size() == 6);
  };

  //
  //
  //
  "At"_test = [] {
    // using U0 = Ts0::At<0>; //! Should not compile.
    static_assert(is::Same<Ts1::At<0>, i8>);
    // using U1 = Ts1::At<1>; //! Should not compile.
    static_assert(is::Same<Ts2::At<0>, i8>);
    static_assert(is::Same<Ts2::At<1>, i16>);
    // using U2 = Ts2::At<2>; //! Should not compile.
    static_assert(is::Same<Ts3::At<0>, i8>);
    static_assert(is::Same<Ts3::At<1>, i16>);
    static_assert(is::Same<Ts3::At<2>, Tss>);
    // using U3 = Ts3::At<3>; //! Should not compile.
    static_assert(is::Same<Ts4::At<0>, i8>);
    static_assert(is::Same<Ts4::At<1>, i16>);
    static_assert(is::Same<Ts4::At<2>, Tss>);
    static_assert(is::Same<Ts4::At<3>, i16>);
    // using U4 = Ts4::At<4>; //! Should not compile.
    static_assert(is::Same<Ts5::At<0>, i8>);
    static_assert(is::Same<Ts5::At<1>, i16>);
    static_assert(is::Same<Ts5::At<2>, Tss>);
    static_assert(is::Same<Ts5::At<3>, i16>);
    static_assert(is::Same<Ts5::At<4>, Tss>);
    // using U5 = Ts5::At<5>; //! Should not compile.
    static_assert(is::Same<Ts6::At<0>, i8>);
    static_assert(is::Same<Ts6::At<1>, i16>);
    static_assert(is::Same<Ts6::At<2>, Tss>);
    static_assert(is::Same<Ts6::At<3>, i16>);
    static_assert(is::Same<Ts6::At<4>, Tss>);
    static_assert(is::Same<Ts6::At<5>, Tss>);
    // using U6 = Ts6::At<6>; //! Should not compile.
  };

  //
  //
  //
  "Front"_test = [] {
    // using U0 = Ts0::Front<>; //! Should not compile.
    static_assert(is::Same<Ts1::Front<>, i8>);
    static_assert(is::Same<Ts2::Front<>, i8>);
    static_assert(is::Same<Ts3::Front<>, i8>);
    static_assert(is::Same<Ts4::Front<>, i8>);
    static_assert(is::Same<Ts5::Front<>, i8>);
    static_assert(is::Same<Ts6::Front<>, i8>);
  };

  //
  //
  //
  "Back"_test = [] {
    // using U0 = Ts0::Back<>; //! Should not compile.
    static_assert(is::Same<Ts1::Back<>, i8>);
    static_assert(is::Same<Ts2::Back<>, i16>);
    static_assert(is::Same<Ts3::Back<>, Tss>);
    static_assert(is::Same<Ts4::Back<>, i16>);
    static_assert(is::Same<Ts5::Back<>, Tss>);
    static_assert(is::Same<Ts6::Back<>, Tss>);
  };

  //
  //
  //
  "Push Front"_test = [] {
    static_assert(is::Same<Ts0::PushFront<void>, Types<void>>);
    static_assert(is::Same<Ts1::PushFront<void>, Types<void, i8>>);
    static_assert(is::Same<Ts2::PushFront<void>, Types<void, i8, i16>>);
    static_assert(is::Same<Ts3::PushFront<void>, Types<void, i8, i16, Tss>>);
    static_assert(is::Same<Ts4::PushFront<void>, Types<void, i8, i16, Tss, i16>>);
    static_assert(is::Same<Ts5::PushFront<void>, Types<void, i8, i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts6::PushFront<void>, Types<void, i8, i16, Tss, i16, Tss, Tss>>);
  };

  //
  //
  //
  "Push Back"_test = [] {
    static_assert(is::Same<Ts0::PushBack<void>, Types<void>>);
    static_assert(is::Same<Ts1::PushBack<void>, Types<i8, void>>);
    static_assert(is::Same<Ts2::PushBack<void>, Types<i8, i16, void>>);
    static_assert(is::Same<Ts3::PushBack<void>, Types<i8, i16, Tss, void>>);
    static_assert(is::Same<Ts4::PushBack<void>, Types<i8, i16, Tss, i16, void>>);
    static_assert(is::Same<Ts5::PushBack<void>, Types<i8, i16, Tss, i16, Tss, void>>);
    static_assert(is::Same<Ts6::PushBack<void>, Types<i8, i16, Tss, i16, Tss, Tss, void>>);
  };

  //
  //
  //
  "Pop Front"_test = [] {
    // using Us0 = Ts0::PopFront<>; //! Should not compile.
    static_assert(is::Same<Ts1::PopFront<>, Types<>>);
    static_assert(is::Same<Ts2::PopFront<>, Types<i16>>);
    static_assert(is::Same<Ts3::PopFront<>, Types<i16, Tss>>);
    static_assert(is::Same<Ts4::PopFront<>, Types<i16, Tss, i16>>);
    static_assert(is::Same<Ts5::PopFront<>, Types<i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts6::PopFront<>, Types<i16, Tss, i16, Tss, Tss>>);
  };

  //
  //
  //
  "Pop Back"_test = [] {
    // using Us0 = Ts0::PopBack<>; //! Should not compile.
    static_assert(is::Same<Ts1::PopBack<>, Types<>>);
    static_assert(is::Same<Ts2::PopBack<>, Types<i8>>);
    static_assert(is::Same<Ts3::PopBack<>, Types<i8, i16>>);
    static_assert(is::Same<Ts4::PopBack<>, Types<i8, i16, Tss>>);
    static_assert(is::Same<Ts5::PopBack<>, Types<i8, i16, Tss, i16>>);
    static_assert(is::Same<Ts6::PopBack<>, Types<i8, i16, Tss, i16, Tss>>);
  };

  //
  //
  //
  "Insert"_test = [] {
    static_assert(is::Same<Ts0::Insert<0, void>, Types<void>>);
    // using Us0 = Ts0::Insert<1, void>; //! Should not compile.
    static_assert(is::Same<Ts1::Insert<0, void>, Types<void, i8>>);
    static_assert(is::Same<Ts1::Insert<1, void>, Types<i8, void>>);
    // using Us1 = Ts1::Insert<2, void>; //! Should not compile.
    static_assert(is::Same<Ts2::Insert<0, void>, Types<void, i8, i16>>);
    static_assert(is::Same<Ts2::Insert<1, void>, Types<i8, void, i16>>);
    static_assert(is::Same<Ts2::Insert<2, void>, Types<i8, i16, void>>);
    // using Us2 = Ts2::Insert<3, void>; //! Should not compile.
    static_assert(is::Same<Ts3::Insert<0, void>, Types<void, i8, i16, Tss>>);
    static_assert(is::Same<Ts3::Insert<1, void>, Types<i8, void, i16, Tss>>);
    static_assert(is::Same<Ts3::Insert<2, void>, Types<i8, i16, void, Tss>>);
    static_assert(is::Same<Ts3::Insert<3, void>, Types<i8, i16, Tss, void>>);
    // using Us3 = Ts3::Insert<4, void>; //! Should not compile.
    static_assert(is::Same<Ts4::Insert<0, void>, Types<void, i8, i16, Tss, i16>>);
    static_assert(is::Same<Ts4::Insert<1, void>, Types<i8, void, i16, Tss, i16>>);
    static_assert(is::Same<Ts4::Insert<2, void>, Types<i8, i16, void, Tss, i16>>);
    static_assert(is::Same<Ts4::Insert<3, void>, Types<i8, i16, Tss, void, i16>>);
    static_assert(is::Same<Ts4::Insert<4, void>, Types<i8, i16, Tss, i16, void>>);
    // using Us4 = Ts4::Insert<5, void>; //! Should not compile.
    static_assert(is::Same<Ts5::Insert<0, void>, Types<void, i8, i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Insert<1, void>, Types<i8, void, i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Insert<2, void>, Types<i8, i16, void, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Insert<3, void>, Types<i8, i16, Tss, void, i16, Tss>>);
    static_assert(is::Same<Ts5::Insert<4, void>, Types<i8, i16, Tss, i16, void, Tss>>);
    static_assert(is::Same<Ts5::Insert<5, void>, Types<i8, i16, Tss, i16, Tss, void>>);
    // using Us5 = Ts5::Insert<6, void>; //! Should not compile.
    static_assert(is::Same<Ts6::Insert<0, void>, Types<void, i8, i16, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Insert<1, void>, Types<i8, void, i16, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Insert<2, void>, Types<i8, i16, void, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Insert<3, void>, Types<i8, i16, Tss, void, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Insert<4, void>, Types<i8, i16, Tss, i16, void, Tss, Tss>>);
    static_assert(is::Same<Ts6::Insert<5, void>, Types<i8, i16, Tss, i16, Tss, void, Tss>>);
    static_assert(is::Same<Ts6::Insert<6, void>, Types<i8, i16, Tss, i16, Tss, Tss, void>>);
    // using Us6 = Ts6::Insert<7, void>; //! Should not compile.
  };

  //
  //
  //
  "Erase"_test = [] {
    // using Us0 = Ts0::Erase<0>; //! Should not compile.
    static_assert(is::Same<Ts1::Erase<0>, Types<>>);
    // using Us1 = Ts1::Erase<1>; //! Should not compile.
    static_assert(is::Same<Ts2::Erase<0>, Types<i16>>);
    static_assert(is::Same<Ts2::Erase<1>, Types<i8>>);
    // using Us2 = Ts2::Erase<2>; //! Should not compile.
    static_assert(is::Same<Ts3::Erase<0>, Types<i16, Tss>>);
    static_assert(is::Same<Ts3::Erase<1>, Types<i8, Tss>>);
    static_assert(is::Same<Ts3::Erase<2>, Types<i8, i16>>);
    // using Us3 = Ts3::Erase<3>; //! Should not compile.
    static_assert(is::Same<Ts4::Erase<0>, Types<i16, Tss, i16>>);
    static_assert(is::Same<Ts4::Erase<1>, Types<i8, Tss, i16>>);
    static_assert(is::Same<Ts4::Erase<2>, Types<i8, i16, i16>>);
    static_assert(is::Same<Ts4::Erase<3>, Types<i8, i16, Tss>>);
    // using Us4 = Ts4::Erase<4>; //! Should not compile.
    static_assert(is::Same<Ts5::Erase<0>, Types<i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Erase<1>, Types<i8, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Erase<2>, Types<i8, i16, i16, Tss>>);
    static_assert(is::Same<Ts5::Erase<3>, Types<i8, i16, Tss, Tss>>);
    static_assert(is::Same<Ts5::Erase<4>, Types<i8, i16, Tss, i16>>);
    // using Us5 = Ts5::Erase<5>; //! Should not compile.
    static_assert(is::Same<Ts6::Erase<0>, Types<i16, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Erase<1>, Types<i8, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Erase<2>, Types<i8, i16, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Erase<3>, Types<i8, i16, Tss, Tss, Tss>>);
    static_assert(is::Same<Ts6::Erase<4>, Types<i8, i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts6::Erase<5>, Types<i8, i16, Tss, i16, Tss>>);
    // using Us6 = Ts6::Erase<6>; //! Should not compile.
  };

  //
  //
  //
  "Remove"_test = [] {
    static_assert(is::Same<Ts0::Remove<void>, Ts0>);
    static_assert(is::Same<Ts0::Remove<i8>, Ts0>);
    static_assert(is::Same<Ts0::Remove<i16>, Ts0>);
    static_assert(is::Same<Ts0::Remove<Tss>, Ts0>);
    static_assert(is::Same<Ts1::Remove<void>, Ts1>);
    static_assert(is::Same<Ts1::Remove<i8>, Types<>>);
    static_assert(is::Same<Ts1::Remove<i16>, Types<i8>>);
    static_assert(is::Same<Ts1::Remove<Tss>, Types<i8>>);
    static_assert(is::Same<Ts2::Remove<void>, Ts2>);
    static_assert(is::Same<Ts2::Remove<i8>, Types<i16>>);
    static_assert(is::Same<Ts2::Remove<i16>, Types<i8>>);
    static_assert(is::Same<Ts2::Remove<Tss>, Types<i8, i16>>);
    static_assert(is::Same<Ts3::Remove<void>, Ts3>);
    static_assert(is::Same<Ts3::Remove<i8>, Types<i16, Tss>>);
    static_assert(is::Same<Ts3::Remove<i16>, Types<i8, Tss>>);
    static_assert(is::Same<Ts3::Remove<Tss>, Types<i8, i16>>);
    static_assert(is::Same<Ts4::Remove<void>, Ts4>);
    static_assert(is::Same<Ts4::Remove<i8>, Types<i16, Tss, i16>>);
    static_assert(is::Same<Ts4::Remove<i16>, Types<i8, Tss>>);
    static_assert(is::Same<Ts4::Remove<Tss>, Types<i8, i16, i16>>);
    static_assert(is::Same<Ts5::Remove<void>, Ts5>);
    static_assert(is::Same<Ts5::Remove<i8>, Types<i16, Tss, i16, Tss>>);
    static_assert(is::Same<Ts5::Remove<i16>, Types<i8, Tss, Tss>>);
    static_assert(is::Same<Ts5::Remove<Tss>, Types<i8, i16, i16>>);
    static_assert(is::Same<Ts6::Remove<void>, Ts6>);
    static_assert(is::Same<Ts6::Remove<i8>, Types<i16, Tss, i16, Tss, Tss>>);
    static_assert(is::Same<Ts6::Remove<i16>, Types<i8, Tss, Tss, Tss>>);
    static_assert(is::Same<Ts6::Remove<Tss>, Types<i8, i16, i16>>);
  };

  //
  //
  //
  "Fill"_test = [] {
    static_assert(is::Same<Ts0::Fill<void, 0>, Types<>>);
    static_assert(is::Same<Ts0::Fill<i8, 1>, Types<i8>>);
    static_assert(is::Same<Ts0::Fill<i16, 2>, Types<i16, i16>>);
    static_assert(is::Same<Ts0::Fill<Tss, 3>, Types<Tss, Tss, Tss>>);
    // static_assert(is::Same<Ts1::Fill<void, 0>, Types<>>); //! Should not compile.
  };

  //
  //
  //
  "Fill Each"_test = [] {
    static_assert(is::Same<Ts0::FillEach<FillEachCase0, 0>, Types<>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase0, 1>, Types<void>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase0, 2>, Types<void, void>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase0, 3>, Types<void, void, void>>);
    // static_assert(is::Same<Ts1::FillEach<FillEachCase0, 0>, Types<>>); //! Should not compile.

    static_assert(is::Same<Ts0::FillEach<FillEachCase1, 0>, Types<>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase1, 1>, Types<C<0>>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase1, 2>, Types<C<0>, C<1>>>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase1, 3>, Types<C<0>, C<1>, C<2>>>);
    // static_assert(is::Same<Ts1::FillEach<FillEachCase1, 0>, Types<>>); //! Should not compile.

    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 0>, Ts0>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 1>, Ts1>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 2>, Ts2>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 3>, Ts3>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 4>, Ts4>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 5>, Ts5>);
    static_assert(is::Same<Ts0::FillEach<FillEachCase2, 6>, Ts6>);
    // static_assert(is::Same<Ts1::FillEach<FillEachCase2, 0>, Ts0>); //! Should not compile.
  };

  //
  //
  //
  "Is And Is Not"_test = [] {
    static_assert(is::Types<Ts0>);
    static_assert(is::Types<Ts1>);
    static_assert(is::Types<Ts2>);
    static_assert(is::Types<Ts3>);
    static_assert(is::Types<Ts4>);
    static_assert(is::Types<Ts5>);
    static_assert(is::Types<Ts6>);

    static_assert(!is::Types<void>);
    static_assert(!is::Types<std::tuple<>>);
    static_assert(!is::Types<std::tuple<void>>);
    static_assert(!is::Types<std::tuple<Ts0>>);

    static_assert(!isnot::Types<Ts0>);
    static_assert(!isnot::Types<Ts1>);
    static_assert(!isnot::Types<Ts2>);
    static_assert(!isnot::Types<Ts3>);
    static_assert(!isnot::Types<Ts4>);
    static_assert(!isnot::Types<Ts5>);
    static_assert(!isnot::Types<Ts6>);

    static_assert(isnot::Types<void>);
    static_assert(isnot::Types<std::tuple<>>);
    static_assert(isnot::Types<std::tuple<void>>);
    static_assert(isnot::Types<std::tuple<Ts0>>);
  };

  //
  //
  //
  "For Each"_test = [] {
    auto label = []<typename Ts> {
      std::array<int, Ts::Size()> vs{};
      usize i{};
      ForEach<Ts>([&]<typename T> {
        if constexpr (is::Same<T, i8>)
          vs[i] = 1;
        else if constexpr (is::Same<T, i16>)
          vs[i] = 2;
        else if constexpr (is::Same<T, Tss>)
          vs[i] = 3;
        else
          vs[i] = -1;
        ++i;
      });
      return vs;
    };

    // Type deductions and correctness.
    ForEach<Ts0>([]<typename T> { static_assert(!is::Same<T, T>); });
    expect(label.template operator()<Ts0>() == std::array<int, 0>{});
    expect(label.template operator()<Ts1>() == std::array{1});
    expect(label.template operator()<Ts2>() == std::array{1, 2});
    expect(label.template operator()<Ts3>() == std::array{1, 2, 3});
    expect(label.template operator()<Ts4>() == std::array{1, 2, 3, 2});
    expect(label.template operator()<Ts5>() == std::array{1, 2, 3, 2, 3});
    expect(label.template operator()<Ts6>() == std::array{1, 2, 3, 2, 3, 3});

    // `constexpr`.
    static_assert(label.template operator()<Ts0>() == std::array<int, 0>{});
    static_assert(label.template operator()<Ts1>() == std::array{1});
    static_assert(label.template operator()<Ts2>() == std::array{1, 2});
    static_assert(label.template operator()<Ts3>() == std::array{1, 2, 3});
    static_assert(label.template operator()<Ts4>() == std::array{1, 2, 3, 2});
    static_assert(label.template operator()<Ts5>() == std::array{1, 2, 3, 2, 3});
    static_assert(label.template operator()<Ts6>() == std::array{1, 2, 3, 2, 3, 3});
  };
};

} // namespace
