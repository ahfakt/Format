#include <StreamFormat/String.hpp>
#include <iostream>
#include <charconv>
#include <numeric>
#include <cstring>
#include <cassert>

int main() {
	using F = float;

	F FLT_MIN = -std::numeric_limits<F>::min();
	F FLT_MAX = -std::numeric_limits<F>::max();

	char buf1[8192] = {};
	char buf2[8192] = {};

	std::size_t precision = 15;
	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN);
		stringOutput << FLT_MIN;
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX);
		stringOutput << FLT_MAX;
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::fixed);
		stringOutput.toChars(FLT_MIN, std::chars_format::fixed);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::fixed);
		stringOutput.toChars(FLT_MAX, std::chars_format::fixed);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::scientific);
		stringOutput.toChars(FLT_MIN, std::chars_format::scientific);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::scientific);
		stringOutput.toChars(FLT_MAX, std::chars_format::scientific);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::general);
		stringOutput.toChars(FLT_MIN, std::chars_format::general);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::general);
		stringOutput.toChars(FLT_MAX, std::chars_format::general);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::hex);
		stringOutput.toChars(FLT_MIN, std::chars_format::hex);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::hex);
		stringOutput.toChars(FLT_MAX, std::chars_format::hex);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::fixed, precision);
		stringOutput.toChars(FLT_MIN, std::chars_format::fixed, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::fixed, precision);
		stringOutput.toChars(FLT_MAX, std::chars_format::fixed, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::scientific, precision);
		stringOutput.toChars(FLT_MIN, std::chars_format::scientific, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::scientific, precision);
		stringOutput.toChars(FLT_MAX, std::chars_format::scientific, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::general, precision);
		stringOutput.toChars(FLT_MIN, std::chars_format::general, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::general, precision);
		stringOutput.toChars(FLT_MAX, std::chars_format::general, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MIN, std::chars_format::hex, precision);
		stringOutput.toChars(FLT_MIN, std::chars_format::hex, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MIN = \t" << buf1 << "\n\t\t\t" << buf2 << "\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	{
		Stream::BufferOutput buffer(buf2, sizeof buf2);
		Stream::Format::StringOutput stringOutput;
		buffer < stringOutput;

		std::to_chars(buf1, buf1 + sizeof buf1, FLT_MAX, std::chars_format::hex, precision);
		stringOutput.toChars(FLT_MAX, std::chars_format::hex, precision);
		assert(std::equal(buf1, buf1 + sizeof buf1, buf2));
		std::cout << "FLT_MAX = \t" << buf1 << "\n\t\t\t" << buf2 << "\n\n";
		std::memset(buf1, 0, sizeof buf1);
		std::memset(buf2, 0, sizeof buf2);
		buffer.resetSpace();
	}

	return 0;
}

