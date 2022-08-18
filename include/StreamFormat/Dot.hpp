#ifndef STREAM_FORMAT_DOT_HPP
#define	STREAM_FORMAT_DOT_HPP

#include "String.hpp"

namespace Stream::Format {

class DotOutput : public StringOutput {
public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	DotOutput() noexcept = default;

	DotOutput(DotOutput&& other) noexcept = default;
};//class Stream::Format::DotOutput

class Dot : public DotOutput {
public:
	struct Exception {
		enum class Code : int {};
	};//struct Stream::Format::Dot::Exception
};//class Stream::Format::Dot

std::error_code
make_error_code(Dot::Exception::Code e) noexcept;

}//namespace Stream::Format

namespace std {

template <>
struct is_error_code_enum<Stream::Format::Dot::Exception::Code> : true_type {};

}//namespace std

#endif //STREAM_FORMAT_DOT_HPP
