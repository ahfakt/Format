#pragma once

#include "String.hpp"

namespace Format {

class DotOutput : public StringOutput {
public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	DotOutput() noexcept = default;

	DotOutput(DotOutput&& other) noexcept = default;
};//class Format::DotOutput

class Dot : public DotOutput {
public:
	struct Exception {
		enum class Code : int {};
	};//struct Format::Dot::Exception
};//class Format::Dot

std::error_code
make_error_code(Dot::Exception::Code e) noexcept;

}//namespace Format

namespace std {

template <>
struct is_error_code_enum<Format::Dot::Exception::Code> : true_type {};

}//namespace std
