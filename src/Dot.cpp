#include "StreamFormat/Dot.hpp"

namespace Stream::Format {

DotOutput::DotOutput(DotOutput&& other) noexcept
{ swap(*this, other); }

std::error_code
make_error_code(Dot::Exception::Code e) noexcept
{
	static struct : std::error_category {
		[[nodiscard]] char const*
		name() const noexcept override
		{ return "Stream::Format::Dot"; }

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
