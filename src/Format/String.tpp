#include "Format/String.hpp"
#include <charconv>
#include <climits>
#include <cstddef>
#include <limits>

namespace Format {

template <typename F>
constexpr std::size_t max_exponent_digits10{0};

template <>
constexpr std::size_t max_exponent_digits10<float>{2}; // FLT_MAX_10_EXP = 38

template <>
constexpr std::size_t max_exponent_digits10<double>{3}; // DBL_MAX_10_EXP = 308

template <>
constexpr std::size_t max_exponent_digits10<long double>{4}; // LDBL_MAX_10_EXP = 4932

template <typename F>
constexpr std::size_t max_exponent_digits2{0};

template <>
constexpr std::size_t max_exponent_digits2<float>{3}; // FLT_MAX_EXP = 128

template <>
constexpr std::size_t max_exponent_digits2<double>{4}; // DBL_MAX_EXP = 1024

template <>
constexpr std::size_t max_exponent_digits2<long double>{5}; // LDBL_MAX_EXP = 16384

template <std::floating_point F>
constexpr std::size_t max_fixed_length{2 + std::numeric_limits<F>::max_digits10 + std::numeric_limits<F>::max_exponent10};

template <std::floating_point F>
constexpr std::size_t max_hex_length{2 + 1 + std::numeric_limits<F>::digits / 4 + 2 + max_exponent_digits2<F>};

template <std::floating_point F>
constexpr std::size_t max_scientific_length{2 + std::numeric_limits<F>::max_digits10 + 2 + max_exponent_digits10<F>};

StringInput&
StringInput::operator>>(Stream::Char auto& c)
{ return reinterpret_cast<StringInput&>(read(&c, sizeof c)); }

StringInput&
StringInput::operator>>(Stream::Integer auto& i)
{ return fromChars(i, 10); }

StringInput&
StringInput::fromChars(Stream::Integer auto& i, unsigned b)
{
	using I = std::remove_reference_t<decltype(i)>;
	if (b >= 2 && b <= 36) {
		if (getDataSize() < std::numeric_limits<I>::digits + (b == 2)) {
			if constexpr (Stream::SignedInt<I>) {
				if (!provideSignedInt(0, std::numeric_limits<I>::digits + (b == 2), b))
					throw Exception{std::make_error_code(std::errc::invalid_argument)};
			} else {
				if (!provideUnsignedInt(0, std::numeric_limits<I>::digits, b))
					throw Exception{std::make_error_code(std::errc::invalid_argument)};
			}
		}
		return checkFromChars(std::from_chars(reinterpret_cast<char const*>(getData()),
			reinterpret_cast<char const*>(getData()) + getDataSize(), i, b));
	}
	throw Exception{std::make_error_code(std::errc::invalid_argument)};
}

StringInput&
StringInput::operator>>(std::floating_point auto& f)
{ return fromChars(f, std::chars_format::general); }

StringInput&
StringInput::fromChars(std::floating_point auto& f, std::chars_format fmt, std::size_t p)
{
	using F = std::remove_reference_t<decltype(f)>;
	std::size_t il;
	std::size_t fl;
	std::size_t el;

	switch (fmt) {
		case std::chars_format::fixed: {
			il = 1 + std::numeric_limits<F>::max_exponent10;
			fl = p ? p : 1 + std::numeric_limits<F>::digits10 + std::numeric_limits<F>::max_exponent10;
			el = 0;
			break;
		}
		case std::chars_format::scientific: {
			il = 1;
			fl = p ? p : 1 + std::numeric_limits<F>::digits10;
			el = max_exponent_digits10<F>;
			break;
		}
		case std::chars_format::general: {
			il = p ? 1 + std::numeric_limits<F>::max_exponent10 : 1;
			fl = p ? p : 1 + std::numeric_limits<F>::digits10;
			el = max_exponent_digits10<F>;
			break;
		}
		case std::chars_format::hex: {
			il = 1;
			fl = p ? p : std::numeric_limits<F>::digits / 4;
			el = max_exponent_digits2<F>;
			break;
		}
		default:
			throw Exception{std::make_error_code(std::errc::invalid_argument)};
	}

	if (getDataSize() < 1 + il + 1 + fl + 2 + el && !provideFloat(0, fmt, il, fl, el))
		throw Exception{std::make_error_code(std::errc::invalid_argument)};

	return checkFromChars(std::from_chars(reinterpret_cast<char const*>(getData()), reinterpret_cast<char const*>(getData()) + getDataSize(), f, fmt));
}

StringOutput&
StringOutput::operator<<(Stream::Char auto c)
{ return reinterpret_cast<StringOutput&>(write(&c, sizeof c)); }

StringOutput&
StringOutput::operator<<(Stream::Integer auto i)
{ return toChars(i, 10); }

StringOutput&
StringOutput::toChars(Stream::Integer auto i, unsigned b)
{
	provideSpace((b == 2) + 8 * sizeof i);
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + getSpaceSize()), i, b));
}

StringOutput&
StringOutput::operator<<(std::floating_point auto f)
{
	using F = decltype(f);
	provideSpace(std::min(max_fixed_length<F>, max_scientific_length<F>));
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + getSpaceSize()), f));
}

StringOutput&
StringOutput::toChars(std::floating_point auto f, std::chars_format fmt)
{
	using F = decltype(f);
	provideSpace(
		fmt == std::chars_format::fixed ? max_fixed_length<F>
		: (fmt == std::chars_format::hex ? max_hex_length<F> : max_scientific_length<F>));
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + getSpaceSize()), f, fmt));
}

StringOutput&
StringOutput::toChars(std::floating_point auto f, std::chars_format fmt, std::size_t precision)
{
	using F = decltype(f);
	provideSpace(2 + precision + 1 + (
		fmt == std::chars_format::fixed ? std::numeric_limits<F>::max_exponent10
		: (fmt == std::chars_format::hex ? 2 + max_exponent_digits2<F> : 2 + max_exponent_digits10<F>)));
	return checkToChars(std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + getSpaceSize()), f, fmt, precision));
}

StringOutput&
StringOutput::operator<<(Stream::Char auto const* s)
{
	using C = std::remove_pointer_t<decltype(s)>;
	return reinterpret_cast<StringOutput&>(write(s, std::char_traits<C>::length(s) * sizeof(C)));
}

template <Stream::Char C>
StringOutput&
StringOutput::operator<<(std::basic_string<C> const& s)
{ return reinterpret_cast<StringOutput&>(write(s.data(), s.size() * sizeof(C))); }

template <Stream::Char C>
StringOutput&
StringOutput::operator<<(std::basic_string_view<C> const& s)
{ return reinterpret_cast<StringOutput&>(write(s.data(), s.size() * sizeof(C))); }

}//namespace Format
