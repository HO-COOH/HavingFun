#ifndef MAKESTRING_HPP
#define MAKESTRING_HPP

#include <string>
#include "dependency/include/NamedType/named_type.hpp"

using fluent::NamedType;

template<typename T>
using Count = NamedType<T, struct CountParameter>;

template<typename T>
using Character = NamedType<T, struct CharacterParameter>;

template<typename T>
using Allocator = NamedType<T, struct AllocatorParameter>;

template<typename T>
using Other = NamedType<T, struct OtherParameter>;

template<typename T>
using Pos = NamedType<T, struct PositionParameter>;

template<typename CharT>
using Pointer = NamedType<CharT const*, struct PointerParameter>;

template<typename InputIt>
using First = NamedType<InputIt, struct FirstParameter>;

template<typename InputIt>
using Last = NamedType<InputIt, struct LastParameter>;

// template<typename CharT = char, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
// inline auto make_string()
// {
//     return std::basic_string<CharT, Traits, Allocator>{};
// }

// template<typename SizeType, typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
// inline auto make_string(Count<SizeType> count, Character<CharT> ch, Allocator const& alloc = Allocator())
// {
//     return std::basic_string<CharT, Traits, Allocator>(count.get(), ch.get(), alloc);
// }

// template<typename CharT, typename Traits, typename Alloc>
// Other(std::basic_string<CharT, Traits, Alloc> const&)->Other<std::basic_string<CharT, Traits, Alloc> const&>;

template<typename T, typename SizeType, typename Alloc>
inline auto make_string(Other<T const&> other, Pos<SizeType> pos, Allocator<Alloc> const& alloc = Allocator{ Alloc() })
{
    return std::basic_string(other.get(), pos.get(), alloc.get());
}

// template<typename CharT, typename Traits, typename Alloc, typename PosSizeType, typename CountSizeType>
// inline auto make_string(Other<std::basic_string<CharT, Traits, Alloc>> const& other, Pos<PosSizeType> pos, Count<CountSizeType> count, Allocator<Alloc> const& alloc = Allocator{ Alloc() })
// {
//     return std::basic_string<CharT, Traits, Alloc>(other.get(), pos.get(), count.get(),alloc.get());
// }

// template<typename CharT, typename SizeType, typename Traits = std::char_traits<CharT>, typename Alloc = std::allocator<CharT>>
// inline auto make_string(Pointer<CharT> s, Count<SizeType> count, Allocator<Alloc> const& alloc = Allocator{ Alloc() })
// {
//     return std::basic_string<CharT, Traits, Alloc>(s.get(), count.get(), alloc.get());
// }

// template<typename CharT, typename Traits = std::char_traits<CharT>, typename Alloc = std::allocator<CharT>>
// inline auto make_string(Pointer<CharT> s, Allocator<Alloc> const& alloc = Alloc())
// {
//     return std::basic_string<CharT, Traits, Alloc>(s.get(), alloc.get());
// }

// template<typename InputIt, typename Alloc = std::allocator<decltype(*InputIt())>>
// inline auto make_string(First<InputIt> first, Last<InputIt> last, Allocator<Alloc const&> alloc = Alloc())
// {
//     return std::basic_string(first.get(), last.get(), alloc.get());
// }

// template<typename CharT, typename Traits, typename Alloc>
// inline auto make_string(Other<std::basic_string<CharT, Traits, Alloc>&&> other)
// {
//     return std::basic_string(std::move(other.get()));
// }

#endif /* MAKESTRING_HPP */
