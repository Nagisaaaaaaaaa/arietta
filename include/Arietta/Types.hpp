#pragma once

/// \file
/// \brief This file defines a compile-time list of types
/// and operations for manipulating it.

//
//
//
//
//
#include "Arietta/Constant.hpp"

namespace arietta {

template <typename... Ts>
struct Types;

namespace detail::types {

template <auto>
struct C {};

template <typename T>
struct Wrapper {
  using type = T;
};

struct Invalid {};

//
//
//
#if 0 //! This implementation has O(n) compile-time complexity.
template <usize i, typename... Ts>
struct AtImpl;

template <typename T, typename... Ts>
struct AtImpl<0, T, Ts...> {
  using type = T;
};

template <usize i, typename T, typename... Ts>
  requires(i > 0)
struct AtImpl<i, T, Ts...> {
  using type = typename AtImpl<i - 1, Ts...>::type;
};
#else //! This implementation has O(1) compile-time complexity.
template <usize i, typename... Ts>
struct AtImpl;

template <usize i>
struct AtImpl<i> {
  template <usize j>
  static consteval auto Type(C<j>) {
    static_assert(j != j, "Index is out of bounds for `Types::At`");
    return Wrapper<Invalid>{};
  }
};

template <usize i, typename T, typename... Ts>
struct AtImpl<i, T, Ts...> : AtImpl<i + 1, Ts...> {
  using AtImpl<i + 1, Ts...>::Type;

  static consteval Wrapper<T> Type(C<i>) { return {}; }
};
#endif

//
//
//
template <auto tag, typename... Ts>
struct FrontImpl;

template <auto tag, typename T, typename... Ts>
struct FrontImpl<tag, T, Ts...> {
  using type = T;
};

//
//
//
template <auto tag, typename... Ts>
struct BackImpl;

template <auto tag, typename T>
struct BackImpl<tag, T> {
  using type = T;
};

template <auto tag, typename T, typename... Ts>
struct BackImpl<tag, T, Ts...> {
  using type = typename BackImpl<tag, Ts...>::type;
};

//
//
//
template <auto tag, typename... Ts>
struct PopFrontImpl;

template <auto tag, typename T, typename... Ts>
struct PopFrontImpl<tag, T, Ts...> {
  using type = Types<Ts...>;
};

//
//
//
template <auto tag, typename... Ts>
struct PopBackImpl;

template <auto tag, typename T>
struct PopBackImpl<tag, T> {
  using type = Types<>;
};

template <auto tag, typename T, typename U, typename... Ts>
struct PopBackImpl<tag, T, U, Ts...> {
  using type = typename PopBackImpl<tag, U, Ts...>::type::template PushFront<T>;
};

//
//
//
template <usize i, typename T, typename... Ts>
struct InsertImpl;

template <typename T, typename... Ts>
struct InsertImpl<0, T, Ts...> {
  using type = Types<T, Ts...>;
};

template <usize i, typename T, typename U, typename... Ts>
  requires(i > 0)
struct InsertImpl<i, T, U, Ts...> {
  using type = typename InsertImpl<i - 1, T, Ts...>::type::template PushFront<U>;
};

//
//
//
template <usize i, typename... Ts>
struct EraseImpl;

template <typename T, typename... Ts>
struct EraseImpl<0, T, Ts...> {
  using type = Types<Ts...>;
};

template <usize i, typename T, typename... Ts>
  requires(i > 0)
struct EraseImpl<i, T, Ts...> {
  using type = typename EraseImpl<i - 1, Ts...>::type::template PushFront<T>;
};

//
//
//
template <typename T, typename... Ts>
struct RemoveImpl;

template <typename T>
struct RemoveImpl<T> {
  using type = Types<>;
};

template <typename T, typename U, typename... Ts>
struct RemoveImpl<T, U, Ts...> {
  using type = std::conditional_t<
      is::Same<T, U>,
      typename RemoveImpl<T, Ts...>::type,
      typename RemoveImpl<T, Ts...>::type::template PushFront<U>>;
};

//
//
//
template <typename T, usize n>
struct FillImpl;

template <typename T>
struct FillImpl<T, 0> {
  using type = Types<>;
};

template <typename T, usize n>
struct FillImpl {
  using type = typename FillImpl<T, n - 1>::type::template PushBack<T>;
};

//
//
//
template <template <usize> typename T, usize n>
struct FillEachImpl;

template <template <usize> typename T>
struct FillEachImpl<T, 0> {
  using type = Types<>;
};

template <template <usize> typename T, usize n>
struct FillEachImpl {
  using type = typename FillEachImpl<T, n - 1>::type::template PushBack<T<n - 1>>;
};

//
//
//
//
//
template <typename... Ts>
struct TypesBase {
  [[nodiscard]] static consteval usize Size() { return sizeof...(Ts); }

#if 0
  template <usize i>
  using At = typename AtImpl<i, Ts...>::type;
#else
  template <usize i>
  using At = typename decltype(AtImpl<0, Ts...>::Type(C<i>{}))::type;
#endif

  template <auto tag = 0>
  using Front = typename FrontImpl<tag, Ts...>::type;

  template <auto tag = 0>
  using Back = typename BackImpl<tag, Ts...>::type;

  template <typename T>
  using PushFront = Types<T, Ts...>;

  template <typename T>
  using PushBack = Types<Ts..., T>;

  template <auto tag = 0>
  using PopFront = typename PopFrontImpl<tag, Ts...>::type;

  template <auto tag = 0>
  using PopBack = typename PopBackImpl<tag, Ts...>::type;

  template <usize i, typename T>
  using Insert = typename InsertImpl<i, T, Ts...>::type;

  template <usize i>
  using Erase = typename EraseImpl<i, Ts...>::type;

  template <typename T>
  using Remove = typename RemoveImpl<T, Ts...>::type;
};

} // namespace detail::types

template <typename... Ts>
struct Types : detail::types::TypesBase<Ts...> {};

template <>
struct Types<> : detail::types::TypesBase<> {
  template <typename T, usize n>
  using Fill = typename detail::types::FillImpl<T, n>::type;

  template <template <usize> typename T, usize n>
  using FillEach = typename detail::types::FillEachImpl<T, n>::type;
};

//
//
//
namespace is {

namespace detail::types {

template <typename>
struct IsTypes : std::false_type {};

template <typename... Ts>
struct IsTypes<Types<Ts...>> : std::true_type {};

} // namespace detail::types

template <typename T>
concept Types = detail::types::IsTypes<T>::value;

} // namespace is

namespace isnot {

template <typename T>
concept Types = !is::Types<T>;

} // namespace isnot

//
//
//
//
//
template <is::Types Ts, typename F>
ART_SPECIFIER constexpr void ForEach(F &&f) {
  ForEach<Ts::Size()>([&]<auto i>() {
    using T = typename Ts::template At<i>;
    static_assert(is::Void<decltype(f.template operator()<T>())>, "Function must have void return type");
    f.template operator()<T>();
  });
}

} // namespace arietta
