#ifndef STREAM_FORMAT_DOT_H
#define	STREAM_FORMAT_DOT_H

#include "StreamFormat/String.h"

namespace Stream::Format {

class DotOutput : public StringOutput {
public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	explicit DotOutput(std::size_t buffInitialSize = 0);

	DotOutput(void* sinkBuff, std::size_t sinkSize) noexcept;

	DotOutput(DotOutput&& other) noexcept;
};

class Dot : public DotOutput {
public:
	struct Exception {
		enum class Code : int {};
	};//struct Exception
};//class Dot

std::error_code
make_error_code(Dot::Exception::Code e) noexcept;

}//namespace Stream::Format

namespace std {

template <>
struct is_error_code_enum<Stream::Format::Dot::Exception::Code> : true_type {};

}//namespace std

#endif //STREAM_FORMAT_DOT_H
