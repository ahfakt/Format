#ifndef STREAM_FORMAT_STRING_HPP
#define	STREAM_FORMAT_STRING_HPP

#include <Stream/Transform.hpp>
#include <charconv>

namespace Stream::Format {

class StringInput : public TransformInput {
public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };
};//class Stream::Format::StringInput

class StringOutput : public TransformOutput {
public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	StringOutput() noexcept = default;

	StringOutput(StringOutput&& other) noexcept;

	template <Char C>
	StringOutput&
	operator<<(std::basic_string<C> const& str);

	StringOutput&
	operator<<(void const* ptr);

	StringOutput&
	operator<<(bool b);

	StringOutput&
	operator<<(Integer auto val);

	StringOutput&
	operator<<(std::floating_point auto val);

	StringOutput&
	toChars(Integer auto val, int base);

	StringOutput&
	toChars(std::floating_point auto val, std::chars_format fmt);

	StringOutput&
	toChars(std::floating_point auto val, std::chars_format fmt, std::size_t precision);
};//class Stream::Format::StringOutput

class String : public StringInput, public StringOutput {
public:
	struct Exception {
		enum class Code : int {
		};
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
