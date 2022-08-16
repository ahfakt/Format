#include <StreamFormat/String.hpp>
#include <iostream>
#include <charconv>
#include <numeric>
#include <cstring>
#include <cassert>
#include <Stream/Pipe.hpp>

int main()
{
	Stream::Pipe pipe;
	Stream::Buffer buffer(pipe.getBufferSize());
	Stream::Format::String str;
	pipe <=> buffer <=> str;

	{
		using U = std::uint64_t;
		using I = std::int64_t;
		U U_MIN = std::numeric_limits<U>::min();
		U U_MAX = std::numeric_limits<U>::max();
		I I_MIN = std::numeric_limits<I>::min();
		I I_MAX = std::numeric_limits<I>::max();
		for (int i = 2; i < 37; ++i) {
			str.toChars(U_MIN, i) << '\n';
			str.toChars(U_MAX, i) << '\n';
			str.toChars(I_MIN, i) << '\n';
			str.toChars(I_MAX, i) << '\n';
		}
		str.flush();
		for (int i = 2; i < 37; ++i) {
			U umin = 0;
			U umax = 0;
			I imin = 0;
			I imax = 0;
			char c;
			str.fromChars(umin, i) >> c;
			str.fromChars(umax, i) >> c;
			str.fromChars(imin, i) >> c;
			str.fromChars(imax, i) >> c;
			assert(umin == U_MIN);
			assert(umax == U_MAX);
			assert(imin == I_MIN);
			assert(imax == I_MAX);
		}
	}

	{
		using U = std::uint32_t;
		using I = std::int32_t;
		U U_MIN = std::numeric_limits<U>::min();
		U U_MAX = std::numeric_limits<U>::max();
		I I_MIN = std::numeric_limits<I>::min();
		I I_MAX = std::numeric_limits<I>::max();
		for (int i = 2; i < 37; ++i) {
			str.toChars(U_MIN, i) << '\n';
			str.toChars(U_MAX, i) << '\n';
			str.toChars(I_MIN, i) << '\n';
			str.toChars(I_MAX, i) << '\n';
		}
		str.flush();
		for (int i = 2; i < 37; ++i) {
			U umin = 0;
			U umax = 0;
			I imin = 0;
			I imax = 0;
			char c;
			str.fromChars(umin, i) >> c;
			str.fromChars(umax, i) >> c;
			str.fromChars(imin, i) >> c;
			str.fromChars(imax, i) >> c;
			assert(umin == U_MIN);
			assert(umax == U_MAX);
			assert(imin == I_MIN);
			assert(imax == I_MAX);
		}
	}

	{
		using U = std::uint16_t;
		using I = std::int16_t;
		U U_MIN = std::numeric_limits<U>::min();
		U U_MAX = std::numeric_limits<U>::max();
		I I_MIN = std::numeric_limits<I>::min();
		I I_MAX = std::numeric_limits<I>::max();
		for (int i = 2; i < 37; ++i) {
			str.toChars(U_MIN, i) << '\n';
			str.toChars(U_MAX, i) << '\n';
			str.toChars(I_MIN, i) << '\n';
			str.toChars(I_MAX, i) << '\n';
		}
		str.flush();
		for (int i = 2; i < 37; ++i) {
			U umin = 0;
			U umax = 0;
			I imin = 0;
			I imax = 0;
			char c;
			str.fromChars(umin, i) >> c;
			str.fromChars(umax, i) >> c;
			str.fromChars(imin, i) >> c;
			str.fromChars(imax, i) >> c;
			assert(umin == U_MIN);
			assert(umax == U_MAX);
			assert(imin == I_MIN);
			assert(imax == I_MAX);
		}
	}

	{
		using U = std::uint8_t;
		using I = std::int8_t;
		U U_MIN = std::numeric_limits<U>::min();
		U U_MAX = std::numeric_limits<U>::max();
		I I_MIN = std::numeric_limits<I>::min();
		I I_MAX = std::numeric_limits<I>::max();
		for (int i = 2; i < 37; ++i) {
			str.toChars(U_MIN, i) << '\n';
			str.toChars(U_MAX, i) << '\n';
			str.toChars(I_MIN, i) << '\n';
			str.toChars(I_MAX, i) << '\n';
		}
		str.flush();
		for (int i = 2; i < 37; ++i) {
			U umin = 0;
			U umax = 0;
			I imin = 0;
			I imax = 0;
			char c;
			str.fromChars(umin, i) >> c;
			str.fromChars(umax, i) >> c;
			str.fromChars(imin, i) >> c;
			str.fromChars(imax, i) >> c;
			assert(umin == U_MIN);
			assert(umax == U_MAX);
			assert(imin == I_MIN);
			assert(imax == I_MAX);
		}
	}

	return 0;
}

