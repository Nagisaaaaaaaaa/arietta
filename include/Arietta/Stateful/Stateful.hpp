#pragma once

/// \file
/// \warning "Humankind cannot gain anything
/// without first giving something in return.
/// To obtain, something of equal value must be lost.
/// That is Alchemy's First Law of Equivalent Exchange."

//
//
//
//
//
#include "Arietta/Arietta.hpp"

namespace arietta::stateful {
namespace {

namespace detail::stateful {

struct Invalid {};

} // namespace detail::stateful

//
//
//
#if defined(SRT_EXPR)
static_assert(false, "Some macros conflict with Arietta");
#endif

/// \brief This macro forces any `consteval` expression
/// with a return type of `auto` to be truly executed at compile time.
#define SRT_EXPR(...)                                                                                                  \
  static_assert(!::std::is_same_v<decltype(__VA_ARGS__), ::arietta::stateful::detail::stateful::Invalid>);

} // namespace
} // namespace arietta::stateful
