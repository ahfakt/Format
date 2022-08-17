#include "StreamFormat/String.hpp"

namespace Stream::Format {

StringInput::StringInput(StringInput&& other) noexcept
{ swap(*this, other); }

StringInput&
StringInput::checkFromChars(std::from_chars_result r)
{
	if (r.ec == std::errc{}) {
		advanceData(reinterpret_cast<std::byte const*>(r.ptr) - getData());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

std::size_t
StringInput::provideFloat(std::size_t i, std::chars_format fmt)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'-'};
		if (fmt != std::chars_format::hex) {
			if (auto r = provideDigits10(j)) {
				j += r;
				j += provideDecFrac(j);
				if (fmt != std::chars_format::fixed)
					j += provideDecExp(j);
			}
		} else {
			if (auto r = provideDigits36(j, 16)) {
				j += r;
				j += provideHexFrac(j);
				j += provideHexExp(j);
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
StringInput::provideSignedInt(std::size_t i, unsigned base)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'-'};
		if (auto r = provideUnsignedInt(j, base))
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideUnsignedInt(std::size_t i, unsigned base)
{ return base <= 10 ? provideDigits10(i, base) : provideDigits36(i, base); }

std::size_t
StringInput::provideDecFrac(std::size_t i)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += getData()[j] == std::byte{'.'};
		if (auto r = provideDigits10(j))
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDecExp(std::size_t i)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		if (getData()[j] == std::byte{'e'} || getData()[j] == std::byte{'E'}) {
			if (++j == getDataSize())
				provideSomeMoreData(1);
			j += getData()[j] == std::byte{'-'} || getData()[j] == std::byte{'+'};
			if (auto r = provideDigits10(j))
				return j + r - i;
		}
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDigits10(std::size_t i, unsigned base)
{
	auto j = i;
	char const d = '0' + base;
	while (true) {
		char const* s = reinterpret_cast<char const*>(getData());
		std::size_t const dataSize = getDataSize();
		for (; j < dataSize; ++j) {
			if (!(s[j] >= '0' && s[j] < d))
				return j - i;
		}
		try {
			provideSomeMoreData(dataSize * 2);
		} catch (Input::Exception const& exc) {
			if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
				throw;
			return j - i;
		}
	}
}

std::size_t
StringInput::provideHexFrac(std::size_t i)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		j += (getData()[j] == std::byte{'.'});
		if (auto r = provideDigits36(j, 16))
			return j + r - i;
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideHexExp(std::size_t i)
{
	auto j = i;
	try {
		if (j == getDataSize())
			provideSomeMoreData(1);
		if (getData()[j] == std::byte{'p'} || getData()[j] == std::byte{'P'}) {
			if (++j == getDataSize())
				provideSomeMoreData(1);
			j += getData()[j] == std::byte{'-'} || getData()[j] == std::byte{'+'};
			if (auto r = provideDigits36(j, 16))
				return j + r - i;
		}
	} catch (Input::Exception const& exc) {
		if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
			throw;
	}
	return 0;
}

std::size_t
StringInput::provideDigits36(std::size_t i, unsigned base)
{
	auto j = i;
	char const l = 'a' + base - 10;
	char const L = 'A' + base - 10;
	while (true) {
		char const* s = reinterpret_cast<char const*>(getData());
		std::size_t const dataSize = getDataSize();
		for (; j < dataSize; ++j) {
			if (!(s[j] >= '0' && s[j] <= '9' || s[j] >= 'a' && s[j] < l || s[j] >= 'A' && s[j] < L))
				return j - i;
		}
		try {
			provideSomeMoreData(dataSize * 2);
		} catch (Input::Exception const& exc) {
			if (exc.code() != std::make_error_code(std::errc::no_message_available) || i == j)
				throw;
			return j - i;
		}
	}
}

StringOutput::StringOutput(StringOutput&& other) noexcept
{ swap(*this, other); }

StringOutput&
StringOutput::checkToChars(std::to_chars_result r)
{
	if (r.ec == std::errc{}) {
		advanceSpace(reinterpret_cast<std::byte*>(r.ptr) - getSpace());
		return *this;
	}
	throw Exception(std::make_error_code(r.ec));
}

StringOutput&
StringOutput::operator<<(void const* ptr)
{ return toChars(reinterpret_cast<std::uintptr_t>(ptr), 16); }

StringOutput&
StringOutput::operator<<(bool b)
{ return reinterpret_cast<StringOutput&>(write(b ? "true" : "false", 5 - b)); }

std::error_code
make_error_code(String::Exception::Code e) noexcept
{
	static struct : std::error_category {
		[[nodiscard]] char const*
		name() const noexcept override
		{ return "Stream::Format::String"; }

		[[nodiscard]] std::string
		message(int e) const noexcept override
		{
			switch (e) {
				default: return "Unknown Error";
			}
		}
	} instance;
	return {static_cast<int>(e), instance};
}

}//namespace Stream::Format
