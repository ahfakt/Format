#ifndef FORMAT_STRING_HPP
#define	FORMAT_STRING_HPP

#include <Stream/Transform.hpp>
#include <charconv>
#include <cstddef>
#include <limits>
#include <string_view>
#include <utility>

namespace Format {

class StringInput : public Stream::TransformInput {
	StringInput&
	checkFromChars(std::from_chars_result r);

	std::size_t
	provideFloat(std::size_t start, std::chars_format fmt, std::size_t intLimit, std::size_t fracLimit, std::size_t expLimit);

	std::size_t
	provideSignedInt(std::size_t start, std::size_t limit, unsigned base = 10);

	std::size_t
	provideUnsignedInt(std::size_t start, std::size_t limit, unsigned base = 10);

	std::size_t
	provideDecFrac(std::size_t start, std::size_t limit);

	std::size_t
	provideDecExp(std::size_t start, std::size_t limit);

	std::size_t
	provideDigits10(std::size_t start, std::size_t limit, unsigned base = 10);

	std::size_t
	provideHexFrac(std::size_t start, std::size_t limit);

	std::size_t
	provideHexExp(std::size_t start, std::size_t limit);

	std::size_t
	provideDigits36(std::size_t start, std::size_t limit, unsigned base = 16);

	std::size_t
	provideUntil(std::size_t start, std::size_t& limit, char delimiter = '\n');

public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };

	StringInput() noexcept = default;

	StringInput(StringInput&& other) noexcept = default;

	StringInput&
	operator>>(bool& b);// TODO:

	StringInput&
	operator>>(Stream::Char auto& c);

	StringInput&
	operator>>(Stream::Integer auto& i);

	StringInput&
	fromChars(Stream::Integer auto& i, unsigned base);

	StringInput&
	operator>>(std::floating_point auto& f);

	StringInput&
	fromChars(std::floating_point auto& f, std::chars_format fmt, std::size_t precision = 0);

	std::tuple<char const*, char const*, char const*>
	getLine(std::size_t limit = std::numeric_limits<std::size_t>::max());

	std::tuple<char const*, char const*, char const*>
	getUntil(char delim = ' ', std::size_t limit = std::numeric_limits<std::size_t>::max());

	std::tuple<char const*, char const*, char const*>
	getUntil(char const* regex = " ", std::size_t limit = std::numeric_limits<std::size_t>::max());
};//class Format::StringInput

class StringOutput : public Stream::TransformOutput {
	StringOutput&
	checkToChars(std::to_chars_result r);

public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	StringOutput() noexcept = default;

	StringOutput(StringOutput&& other) noexcept = default;

	StringOutput&
	operator<<(void const* p);

	StringOutput&
	operator<<(bool b);

	StringOutput&
	operator<<(Stream::Char auto c);

	StringOutput&
	operator<<(Stream::Integer auto i);

	StringOutput&
	toChars(Stream::Integer auto i, unsigned base);

	StringOutput&
	operator<<(std::floating_point auto f);

	StringOutput&
	toChars(std::floating_point auto f, std::chars_format fmt);

	StringOutput&
	toChars(std::floating_point auto f, std::chars_format fmt, std::size_t precision);

	StringOutput&
	operator<<(Stream::Char auto const* s);

	template <Stream::Char C>
	StringOutput&
	operator<<(std::basic_string<C> const& s);

	template <Stream::Char C>
	StringOutput&
	operator<<(std::basic_string_view<C> const& s);
};//class Format::StringOutput

class String : public StringInput, public StringOutput {
public:
	struct Exception {
		enum class Code : int {};
	};//struct Format::String::Exception

	struct UppercaseHash {
		std::size_t
		operator()(std::string const& h) const;
	};//struct Format::String::UppercaseHash

	struct CaseInsensitiveEqualTo {
		bool
		operator()(std::string const& a, std::string const& b) const noexcept;
	};//struct Format::String::CaseInsensitiveEqualTo
};//class Format::String

std::error_code
make_error_code(String::Exception::Code e) noexcept;

std::string_view
ToStringView(std::tuple<char const*, char const*, char const*> const& triplet);

std::string
ToString(std::tuple<char const*, char const*, char const*> const& triplet);

}//namespace Format

namespace std {

template <>
struct is_error_code_enum<Format::String::Exception::Code> : true_type {};

}//namespace std

#include "../../src/Format/String.tpp"

#endif //FORMAT_STRING_HPP
