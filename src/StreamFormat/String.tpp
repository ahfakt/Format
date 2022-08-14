#include "StreamFormat/String.hpp"
#include <climits>

namespace Stream::Format {

template <Char C>
StringOutput&
StringOutput::operator<<(std::basic_string<C> const& str)
{ return reinterpret_cast<StringOutput&>(write(str.data(), str.size() * sizeof(C))); }

template <typename F>
constexpr int max_exponent_digits10 = 0;

template <>
constexpr int max_exponent_digits10<float> = 2; // FLT_MAX_10_EXP = 38

template <>
constexpr int max_exponent_digits10<double> = 3; // DBL_MAX_10_EXP = 308

template <>
constexpr int max_exponent_digits10<long double> = 4; // LDBL_MAX_10_EXP = 4932

template <typename F>
constexpr int max_exponent_digits2 = 0;

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

StringOutput&
StringOutput::operator<<(Integer auto val)
{ return toChars(val, 10); }

StringOutput&
StringOutput::operator<<(std::floating_point auto val)
{
	constexpr std::size_t max = std::min(max_fixed_length<decltype(val)>, max_scientific_length<decltype(val)>);
	provideSpace(max);
	std::to_chars_result r = std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), val);
	if (r.ec == std::errc()) {
		advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

StringOutput&
StringOutput::toChars(Integer auto val, int base)
{
	constexpr std::size_t max = 1 + 8 * sizeof val;
	provideSpace(max);
	std::to_chars_result r = std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), val, base);
	if (r.ec == std::errc()) {
		advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

StringOutput&
StringOutput::toChars(std::floating_point auto val, std::chars_format fmt)
{
	std::size_t max = fmt == std::chars_format::fixed ? max_fixed_length<decltype(val)>
			: (fmt == std::chars_format::hex ? max_hex_length<decltype(val)>
			: max_scientific_length<decltype(val)>);
	provideSpace(max);
	std::to_chars_result r = std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), val, fmt);
	if (r.ec == std::errc()) {
		advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

StringOutput&
StringOutput::toChars(std::floating_point auto val, std::chars_format fmt, std::size_t precision)
{
	std::size_t max = 2 + precision + 1;
	max += fmt == std::chars_format::fixed ? std::numeric_limits<decltype(val)>::max_exponent10
			: (fmt == std::chars_format::hex ? 2 + max_exponent_digits2<decltype(val)>
			: 2 + max_exponent_digits10<decltype(val)>);
	provideSpace(max);
	std::to_chars_result r = std::to_chars(reinterpret_cast<char*>(getSpace()), reinterpret_cast<char*>(getSpace() + max), val, fmt, precision);
	if (r.ec == std::errc()) {
		advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

}//namespace Stream::Format
