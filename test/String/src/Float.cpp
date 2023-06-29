#include <Stream/Pipe.hpp>
#include <Format/String.hpp>
#include <iostream>
#include <charconv>
#include <numeric>
#include <cstring>
#include <cassert>

int main()
{
	Stream::Pipe pipe;
	Stream::Buffer buffer{100 * pipe.getBufferSize()};
	Format::String str;
	pipe <=> buffer <=> str;
	{
		using F = float;
		F FLT_MIN{-std::numeric_limits<F>::min()};
		F FLT_MAX{-std::numeric_limits<F>::max()};
		std::size_t precision{60};

		str.toChars(FLT_MIN, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex) << '\n';
		str.toChars(FLT_MIN, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex, precision) << '\n';
		str.flush();

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed) >> c;
			str.fromChars(max, std::chars_format::fixed) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific) >> c;
			str.fromChars(max, std::chars_format::scientific) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general) >> c;
			str.fromChars(max, std::chars_format::general) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex) >> c;
			str.fromChars(max, std::chars_format::hex) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed, precision) >> c;
			str.fromChars(max, std::chars_format::fixed, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific, precision) >> c;
			str.fromChars(max, std::chars_format::scientific, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general, precision) >> c;
			str.fromChars(max, std::chars_format::general, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex, precision) >> c;
			str.fromChars(max, std::chars_format::hex, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}
	}

	{
		using F = double;
		F FLT_MIN{-std::numeric_limits<F>::min()};
		F FLT_MAX{-std::numeric_limits<F>::max()};
		std::size_t precision{400};

		str.toChars(FLT_MIN, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex) << '\n';
		str.toChars(FLT_MIN, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex, precision) << '\n';
		str.flush();

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed) >> c;
			str.fromChars(max, std::chars_format::fixed) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific) >> c;
			str.fromChars(max, std::chars_format::scientific) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general) >> c;
			str.fromChars(max, std::chars_format::general) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex) >> c;
			str.fromChars(max, std::chars_format::hex) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed, precision) >> c;
			str.fromChars(max, std::chars_format::fixed, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific, precision) >> c;
			str.fromChars(max, std::chars_format::scientific, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general, precision) >> c;
			str.fromChars(max, std::chars_format::general, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex, precision) >> c;
			str.fromChars(max, std::chars_format::hex, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}
	}

	{
		using F = long double;
		F FLT_MIN{-std::numeric_limits<F>::min()};
		F FLT_MAX{-std::numeric_limits<F>::max()};
		std::size_t precision{6000};

		str.toChars(FLT_MIN, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex) << '\n';
		str.toChars(FLT_MIN, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::fixed, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::scientific, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::general, precision) << '\n';
		str.toChars(FLT_MIN, std::chars_format::hex, precision) << '\n';
		str.toChars(FLT_MAX, std::chars_format::hex, precision) << '\n';
		str.flush();

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed) >> c;
			str.fromChars(max, std::chars_format::fixed) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific) >> c;
			str.fromChars(max, std::chars_format::scientific) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general) >> c;
			str.fromChars(max, std::chars_format::general) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex) >> c;
			str.fromChars(max, std::chars_format::hex) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::fixed, precision) >> c;
			str.fromChars(max, std::chars_format::fixed, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::scientific, precision) >> c;
			str.fromChars(max, std::chars_format::scientific, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::general, precision) >> c;
			str.fromChars(max, std::chars_format::general, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}

		{
			F min{0};
			F max{0};
			char c;
			str.fromChars(min, std::chars_format::hex, precision) >> c;
			str.fromChars(max, std::chars_format::hex, precision) >> c;
			assert(min == FLT_MIN);
			assert(max == FLT_MAX);
		}
	}

	return 0;
}

