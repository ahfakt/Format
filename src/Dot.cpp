#include "Format/Dot.hpp"

namespace Format {

std::error_code
make_error_code(Dot::Exception::Code e) noexcept
{
	static const struct : std::error_category {
		[[nodiscard]] char const*
		name() const noexcept override
		{ return "Format::Dot"; }

		[[nodiscard]] std::string
		message(int e) const noexcept override
		{
			switch (e) {
			default: return "Unknown Error";
			}
		}
	} cat;
	return {static_cast<int>(e), cat};
}

}//namespace Format
