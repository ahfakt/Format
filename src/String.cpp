#include "Format/String.hpp"
#include <cstddef>
#include <string_view>
#include <system_error>

namespace Format {

StringInput&
StringInput::checkFromChars(std::from_chars_result r)
{
	if (r.ec != std::errc{})
		throw Exception{std::make_error_code(r.ec)};
	advanceData(reinterpret_cast<std::byte const*>(r.ptr) - getData());
	return *this;
}

std::size_t
StringInput::provideFloat(std::size_t i, std::chars_format fmt, std::size_t il, std::size_t fl, std::size_t el)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'-'};
		if (fmt != std::chars_format::hex) {
			if (auto r{provideDigits10(j, il)}) try {
				j += provideDecFrac(j += r, fl);
				if (fmt != std::chars_format::fixed)
					j += provideDecExp(j, el);
			} catch (Input::Exception const& exc) {
				if (exc.code() != std::make_error_code(std::errc::no_message_available))
					throw;
			}
		} else {
			if (auto r{provideDigits36(j, il)}) try {
				j += provideHexFrac(j += r, fl);
				j += provideHexExp(j, el);
			} catch (Input::Exception const& exc) {
				if (exc.code() != std::make_error_code(std::errc::no_message_available))
					throw;
			}
		}
		return j - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideSignedInt(std::size_t i, std::size_t l, unsigned b)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'-'};
		if (auto r{provideUnsignedInt(j, l, b)})
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideUnsignedInt(std::size_t i, std::size_t l, unsigned b)
{ return b <= 10 ? provideDigits10(i, l, b) : provideDigits36(i, l, b); }

std::size_t
StringInput::provideDecFrac(std::size_t i, std::size_t l)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'.'};
		if (auto r{provideDigits10(j, l)})
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDecExp(std::size_t i, std::size_t l)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		if (getData()[j] == std::byte{'e'} || getData()[j] == std::byte{'E'}) {
			if (++j == getDataSize())
				provideSomeMoreData(1);
			j += getData()[j] == std::byte{'-'} || getData()[j] == std::byte{'+'};
			if (auto r{provideDigits10(j, l)})
				return j + r - i;
		}
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDigits10(std::size_t i, std::size_t l, unsigned b)
{
	l += i;
	auto j{i};
	char const d = '0' + b;
	while (true) {
		char const* s{reinterpret_cast<char const*>(getData())};
		auto const e = getDataSize();
		for (; j < e; ++j) {
			if (!(s[j] >= '0' && s[j] < d))
				return j - i;
			if (j == l)
				throw Exception{std::make_error_code(std::errc::result_out_of_range)};
		}
		try {
			provideSomeMoreData(e * 2);
		} catch (Input::Exception const& exc) {
			if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
				throw;
			return j - i;
		}
	}
}

std::size_t
StringInput::provideHexFrac(std::size_t i, std::size_t l)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += (getData()[j] == std::byte{'.'});
		if (auto r{provideDigits36(j, l)})
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideHexExp(std::size_t i, std::size_t l)
{
	auto j{i};
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		if (getData()[j] == std::byte{'p'} || getData()[j] == std::byte{'P'}) {
			if (++j == getDataSize())
				provideSomeMoreData(1);
			j += getData()[j] == std::byte{'-'} || getData()[j] == std::byte{'+'};
			if (auto r{provideDigits36(j, l)})
				return j + r - i;
		}
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDigits36(std::size_t i, std::size_t l, unsigned b)
{
	l += i;
	auto j{i};
	char const d = 'a' + b - 10;
	char const D = 'A' + b - 10;
	while (true) {
		char const* s{reinterpret_cast<char const*>(getData())};
		auto const e = getDataSize();
		for (; j < e; ++j) {
			if (!(s[j] >= '0' && s[j] <= '9' || s[j] >= 'a' && s[j] < d || s[j] >= 'A' && s[j] < D))
				return j - i;
			if (j == l)
				throw Exception{std::make_error_code(std::errc::result_out_of_range)};
		}
		try {
			provideSomeMoreData(e * 2);
		} catch (Input::Exception const& exc) {
			if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
				throw;
			return j - i;
		}
	}
}

std::size_t
StringInput::provideUntil(std::size_t const i, std::size_t& l, char d)
{
	l += i;
	auto j{i};
	while (true) {
		char const* s{reinterpret_cast<char const*>(getData())};
		auto const e = getDataSize();
		for (; j < e; ++j) {
			if (s[j] == d)
				return (l = j - i) + 1;
			if (j == l)
				throw Exception{std::make_error_code(std::errc::result_out_of_range)};
		}
		try {
			provideSomeMoreData(e * 2);
		} catch (Input::Exception const& exc) {
			if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
				throw;
			return l = j - i;
		}
	}
}

std::tuple<char const*, char const*, char const*>
StringInput::getLine(std::size_t l)
{
	auto e{provideUntil(0, l, '\n')};
	if (e > 1 && getData()[e - 2] == std::byte{'\r'})
		--l;
	char const* b{reinterpret_cast<char const*>(getData())};
	advanceData(e);
	return {b, b + l, b + e};
}

std::tuple<char const*, char const*, char const*>
StringInput::getUntil(char d, std::size_t l)
{
	auto e{provideUntil(0, l, d)};
	char const* b{reinterpret_cast<char const*>(getData())};
	advanceData(e);
	return {b, b + l, b + e};
}

StringOutput&
StringOutput::checkToChars(std::to_chars_result r)
{
	if (r.ec != std::errc{})
		throw Exception{std::make_error_code(r.ec)};
	advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
	return *this;
}

StringOutput&
StringOutput::operator<<(void const* ptr)
{ return toChars(reinterpret_cast<std::uintptr_t>(ptr), 16); }

StringOutput&
StringOutput::operator<<(bool b)
{ return reinterpret_cast<StringOutput&>(write(b ? "true" : "false", 5 - b)); }

std::size_t
String::UppercaseHash::operator()(std::string const& h) const
{	// Convert to uppercase before calculating the hash value
	auto upper{h};
	for (auto& c : upper)
		c &= static_cast<char>(0b11011111);
	return std::hash<std::string>{}(upper);
}

bool
String::CaseInsensitiveEqualTo::operator()(std::string const& a, std::string const& b) const noexcept
{	// Case-insensitive comparison
	std::string::size_type sz{a.size()};
	if (b.size() != sz)
		return false;
	for (std::string::size_type i{0}; i < sz; ++i)
		if ((a[i] ^ b[i]) & static_cast<char>(0b11011111))
			return false;
	return true;
}

std::error_code
make_error_code(String::Exception::Code e) noexcept
{
	static const struct : std::error_category {
		[[nodiscard]] char const*
		name() const noexcept override
		{ return "Format::String"; }

		[[nodiscard]] std::string
		message(int e) const noexcept override
		{
			using namespace std::string_literals;
			switch (static_cast<String::Exception::Code>(e)) {
				default: return "Unknown Error"s;
			}
		}
	} cat;
	return {static_cast<int>(e), cat};
}

std::string_view
ToStringView(std::tuple<char const*, char const*, char const*> const& t)
{ return {std::get<0>(t), std::get<1>(t)}; }

std::string
ToString(std::tuple<char const*, char const*, char const*> const& t)
{ return {std::get<0>(t), std::get<1>(t)}; }

}//namespace Format
