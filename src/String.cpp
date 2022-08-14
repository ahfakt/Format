#include "StreamFormat/String.hpp"

namespace Stream::Format {

StringOutput::StringOutput(StringOutput&& other) noexcept
{ swap(*this, other); }

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
