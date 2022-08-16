#ifndef STREAM_FORMAT_STRING_HPP
#define	STREAM_FORMAT_STRING_HPP

#include <Stream/Transform.hpp>
#include <charconv>

namespace Stream::Format {

class StringInput : public TransformInput {
	StringInput&
	checkFromChars(std::from_chars_result r);

	std::size_t
	provideFloat(std::size_t start, std::chars_format fmt);

	std::size_t
	provideSignedInt(std::size_t start, unsigned base = 10);

	std::size_t
	provideUnsignedInt(std::size_t start, unsigned base = 10);

	std::size_t
	provideDecFrac(std::size_t start = 0);

	std::size_t
	provideDecExp(std::size_t start = 0);

	std::size_t
	provideDigits10(std::size_t start, unsigned base = 10);

	std::size_t
	provideHexFrac(std::size_t start = 0);

	std::size_t
	provideHexExp(std::size_t start = 0);

	std::size_t
	provideDigits36(std::size_t start, unsigned base = 10);

public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };

	StringInput() noexcept = default;

	StringInput(StringInput&& other) noexcept;

	StringInput&
	operator>>(bool& b);

	StringInput&
	operator>>(Char auto& c);

	StringInput&
	operator>>(Integer auto& i);

	StringInput&
	fromChars(Integer auto& i, unsigned base);

	StringInput&
	operator>>(std::floating_point auto& f);

	StringInput&
	fromChars(std::floating_point auto& f, std::chars_format fmt);
};//class Stream::Format::StringInput

class StringOutput : public TransformOutput {
	StringOutput&
	checkToChars(std::to_chars_result r);

public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	StringOutput() noexcept = default;

	StringOutput(StringOutput&& other) noexcept;

	StringOutput&
	operator<<(void const* p);

	StringOutput&
	operator<<(bool b);

	StringOutput&
	operator<<(Char auto c);

	StringOutput&
	operator<<(Integer auto i);

	StringOutput&
	toChars(Integer auto i, unsigned base);

	StringOutput&
	operator<<(std::floating_point auto f);

	StringOutput&
	toChars(std::floating_point auto f, std::chars_format fmt);

	StringOutput&
	toChars(std::floating_point auto f, std::chars_format fmt, std::size_t precision);

	template <Char C>
	StringOutput&
	operator<<(std::basic_string<C> const& s);
};//class Stream::Format::StringOutput

class String : public StringInput, public StringOutput {
public:
	struct Exception {
		enum class Code : int {};
	};//struct Stream::Format::String::Exception
};//class Stream::Format::String

std::error_code
make_error_code(String::Exception::Code e) noexcept;

}//namespace Stream::Format

namespace std {

template <>
struct is_error_code_enum<Stream::Format::String::Exception::Code> : true_type {};

}//namespace std

#include "../../src/StreamFormat/String.tpp"

#endif //STREAM_FORMAT_STRING_HPP
