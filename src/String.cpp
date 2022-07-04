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

StringOutput&
StringOutput::operator<<(std::string const& str)
{ return reinterpret_cast<StringOutput&>(write(str.c_str(), str.size())); }

StringOutput&
StringOutput::operator<<(char const* str)
{ return reinterpret_cast<StringOutput&>(write(str, std::strlen(str))); }

StringOutput&
StringOutput::operator<<(bool b)
{ return reinterpret_cast<StringOutput&>(write(b ? "true" : "false", 5 - b)); }

StringOutput&
StringOutput::operator<<(void const* ptr)
{ return toChars(reinterpret_cast<std::uintptr_t>(ptr), 16); }

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
