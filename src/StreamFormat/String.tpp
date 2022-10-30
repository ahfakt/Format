#include "StreamFormat/String.hpp"
#include <climits>

namespace Stream::Format {

template <typename F>
constexpr int max_exponent_digits10{0};

template <>
constexpr int max_exponent_digits10<float> = 2; // FLT_MAX_10_EXP = 38

template <>
constexpr int max_exponent_digits10<double> = 3; // DBL_MAX_10_EXP = 308

template <>
constexpr int max_exponent_digits10<long double> = 4; // LDBL_MAX_10_EXP = 4932

template <typename F>
constexpr int max_exponent_digits2{0};

template <>
constexpr int max_exponent_digits2<float> = 3; // FLT_MAX_EXP = 128

template <>
constexpr int max_exponent_digits2<double> = 4; // DBL_MAX_EXP = 1024

template <>
constexpr int max_exponent_digits2<long double> = 5; // LDBL_MAX_EXP = 16384

template <std::floating_point F>
constexpr int max_fixed_length = 2 + std::numeric_limits<F>::max_digits10 + std::numeric_limits<F>::max_exponent10;

template <std::floating_point F>
constexpr int max_hex_length = 2 + 1 + std::numeric_limits<F>::digits / 4 + 2 + max_exponent_digits2<F>;

template <std::floating_point F>
constexpr int max_scientific_length = 2 + std::numeric_limits<F>::max_digits10 + 2 + max_exponent_digits10<F>;

StringInput&
StringInput::operator>>(Char auto& c)
{ return reinterpret_cast<StringInput&>(read(&c, sizeof c)); }

StringInput&
StringInput::operator>>(Integer auto& i)
{ return fromChars(i, 10); }

StringInput&
StringInput::fromChars(Integer auto& i, unsigned base)
{
	if (base >= 2 && base <= 36) {
		if constexpr (SignedInt<std::remove_reference_t<decltype(i)>>) {
			if (auto e = provideSignedInt(0, base))
				return checkFromChars(std::from_chars(reinterpret_cast<char const*>(getData()),
						reinterpret_cast<char const*>(getData()) + e, i, base));
		} else {
			if (auto e = provideUnsignedInt(0, base))
				return checkFromChars(std::from_chars(reinterpret_cast<char const*>(getData()),
						reinterpret_cast<char const*>(getData()) + e, i, base));
		}
	}
	throw Exception(std::make_error_code(std::errc::invalid_argument));
}

StringInput&
StringInput::operator>>(std::floating_point auto& f)
{ return fromChars(f, std::chars_format::general); }

StringInput&
StringInput::fromChars(std::floating_point auto& f, std::chars_format fmt)
{
	if (auto e = provideFloat(0, fmt))
		return checkFromChars(std::from_chars(reinterpret_cast<char const*>(getData()),
				reinterpret_cast<char const*>(getData()) + e, f, fmt));
	throw Exception(std::make_error_code(std::errc::invalid_argument));
}

StringOutput&
StringOutput::operator<<(Char auto c)
{ return reinterpret_cast<StringOutput&>(write(&c, sizeof c)); }

StringOutput&
StringOutput::operator<<(Integer auto i)
{ return toChars(i, 10); }

StringOutput&
StringOutput::toChars(Integer auto i, unsigned base)
{
	constexpr std::size_t max = 1 + 8 * sizeof i;
	provideSpace(max);
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), i, base));
}

StringOutput&
StringOutput::operator<<(std::floating_point auto f)
{
	constexpr std::size_t max = std::min(max_fixed_length<decltype(f)>, max_scientific_length<decltype(f)>);
	provideSpace(max);
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), f));
}

StringOutput&
StringOutput::toChars(std::floating_point auto f, std::chars_format fmt)
{
	std::size_t max = fmt == std::chars_format::fixed ? max_fixed_length<decltype(f)>
			: (fmt == std::chars_format::hex ? max_hex_length<decltype(f)>
			: max_scientific_length<decltype(f)>);
	provideSpace(max);
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), f, fmt));
}

StringOutput&
StringOutput::toChars(std::floating_point auto f, std::chars_format fmt, std::size_t precision)
{
	std::size_t max = 2 + precision + 1;
	max += fmt == std::chars_format::fixed ? std::numeric_limits<decltype(f)>::max_exponent10
			: (fmt == std::chars_format::hex ? 2 + max_exponent_digits2<decltype(f)>
			: 2 + max_exponent_digits10<decltype(f)>);
	provideSpace(max);
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), f, fmt, precision));
}

template <Char C>
StringOutput&
StringOutput::operator<<(std::basic_string<C> const& s)
{ return reinterpret_cast<StringOutput&>(write(s.data(), s.size() * sizeof(C))); }

StringOutput&
StringOutput::operator<<(Char auto const* s)
{
	using C = std::remove_pointer_t<decltype(s)>;
	return reinterpret_cast<StringOutput&>(write(s, std::char_traits<C>::length(s) * sizeof(C)));
}

}//namespace Stream::Format
