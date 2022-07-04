#include "StreamFormat/String.h"

namespace Stream::Format {

StringOutput::StringOutput(std::size_t buffInitialSize)
		: BufferOutput(buffInitialSize)
{}

StringOutput::StringOutput(void* sinkBuff, std::size_t sinkSize) noexcept
		: BufferOutput(sinkBuff, sinkSize)
{}

StringOutput::StringOutput(StringOutput&& other) noexcept
{ swap(*this, other); }

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
