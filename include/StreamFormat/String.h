#ifndef STREAM_FORMAT_STRING_H
#define	STREAM_FORMAT_STRING_H

#include "Stream/Buffer.h"
#include <charconv>

namespace Stream::Format {

template <typename I>
concept SignedInt = std::is_same_v<I, signed char>
	|| std::is_same_v<I, signed short>
	|| std::is_same_v<I, signed int>
	|| std::is_same_v<I, signed long>
	|| std::is_same_v<I, signed long long>;

template <typename I>
concept UnsignedInt = std::is_same_v<I, unsigned char>
	|| std::is_same_v<I, unsigned short>
	|| std::is_same_v<I, unsigned int>
	|| std::is_same_v<I, unsigned long>
	|| std::is_same_v<I, unsigned long long>;

template <typename I>
concept Integer = SignedInt<I> || UnsignedInt<I>;

class StringInput : public BufferInput {
public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };
};//class StringInput

class StringOutput : public BufferOutput {
public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	explicit StringOutput(std::size_t buffInitialSize = 0);

	StringOutput(void* sinkBuff, std::size_t sinkSize) noexcept;

	StringOutput(StringOutput&& other) noexcept;

	StringOutput&
	toChars(Integer auto val, int base);

	StringOutput&
	toChars(std::floating_point auto val);

	StringOutput&
	toChars(std::floating_point auto val, std::chars_format fmt);

	StringOutput&
	toChars(std::floating_point auto val, std::chars_format fmt, std::size_t precision);
};//class StringOutput

class String : public StringInput, public StringOutput {
public:
	struct Exception {
		enum class Code : int {
		};
	};//struct Exception
};//class String

std::error_code
make_error_code(String::Exception::Code e) noexcept;

}//namespace Stream::Format

namespace std {

template <>
struct is_error_code_enum<Stream::Format::String::Exception::Code> : true_type {};

}//namespace std

#include "../src/String.hpp"

#endif //STREAM_FORMAT_STRING_H
