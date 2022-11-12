#include "Stream/Buffer.hpp"
#include "Stream/InOut.hpp"
#include <Stream/Pipe.hpp>
#include <Format/String.hpp>
#include <iostream>
#include <charconv>
#include <numeric>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <system_error>

int main()
{

	Stream::BufferInput buffer("First Line\r\n\r\nThird Line\n\n", 26);
	Format::StringInput str;

	buffer > str;

	assert((str.getLine() == "First Line"));
	assert((str.getLine() == ""));
	assert((str.getLine() == "Third Line"));
	assert((str.getLine() == ""));

	try {
		str.getLine();
	} catch (Stream::Input::Exception const& exc) {
		assert((exc.code() == std::make_error_code(std::errc::no_message_available)));
	}

	return 0;
}

