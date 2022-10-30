#ifndef STREAM_FORMAT_BASE_HPP
#define	STREAM_FORMAT_BASE_HPP

#include <Stream/Transform.hpp>
#include <memory>

namespace Stream::Format {

enum class BaseMode : short {
	B_ALPHABET2ND		= 1,
	B_ALPHABET3RD		= 2,
	B_ALPHABET4TH		= 3,
	B_ALPHABET_MASK		= 3,
	B_NPAD				= 4, // no padding
	B_PAD				= 8,
	B_OPAD				= B_NPAD | B_PAD, // optional padding
	//B_IGNORE_INVALID 	= 128,
	//B_SPLITTED			= 256,
	BASE16				= 16 | B_NPAD,
	BASE32				= 32 | B_OPAD,
	BASE32_NPAD			= 32 | B_NPAD,
	BASE32_HEX			= BASE32 | B_ALPHABET2ND,
	BASE32_HEX_NPAD		= BASE32_NPAD | B_ALPHABET2ND,
	BASE64				= 64 | B_OPAD,
	BASE64_NPAD			= 64 | B_NPAD,
	BASE64_URL			= BASE64 | B_ALPHABET2ND,
	BASE64_URL_NPAD		= BASE64_NPAD | B_ALPHABET2ND,
	BASE64_UTF7			= BASE64_NPAD,
	BASE64_IMAP			= BASE64_NPAD | B_ALPHABET3RD,
	//BASE64_PEM		= 64 | B_PAD | B_SPLITTED, // todo line length 64
	//BASE64_MIME		= 64 | B_PAD | B_SPLITTED | B_IGNORE_INVALID, // todo line length 76
};

/**
 * @brief	Stream::Input %Base decoder
 * @class	BaseDecode Base.hpp "StreamFormat/Base.hpp"
 */
class BaseDecode : public TransformInput {
	std::unique_ptr<unsigned char> mCtx;
	std::unique_ptr<unsigned char> mTempBeg;
	unsigned char* mTempCurr{nullptr};
	unsigned char const* mTempEnd{nullptr};
	bool mFinalizeWhenNoData = true;

	std::size_t
	readBytes(std::byte* dest, std::size_t size) override;

	void
	init(short mode);

public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };

	explicit BaseDecode(BaseMode mode);

	BaseDecode(BaseDecode&& other) noexcept;

	friend void
	swap(BaseDecode& a, BaseDecode& b) noexcept;

	BaseDecode&
	operator=(BaseDecode&& other) noexcept;

	void
	finalizeDecoding();

	void
	finalizeDecodingWhenNoData(bool on = true);
};//class Stream::Format::BaseDecode

/**
 * @brief	Stream::Output %Base encoder
 * @class	BaseEncode Base.hpp "StreamFormat/Base.hpp"
 * @see		https://tools.ietf.org/html/rfc4648
 */
class BaseEncode : public TransformOutput {
	std::unique_ptr<unsigned char> mCtx;

	std::size_t
	writeBytes(std::byte const* src, std::size_t size) override;

	void
	init(short mode);

public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	explicit BaseEncode(BaseMode mode);

	BaseEncode(BaseEncode&& other) noexcept;

	friend void
	swap(BaseEncode& a, BaseEncode& b) noexcept;

	BaseEncode&
	operator=(BaseEncode&& other) noexcept;

	~BaseEncode();

	void
	finalizeEncoding();
};//class Stream::Format::BaseEncode

/**
 * @brief Stream::Input / Stream::Output %Base decoder and encoder
 * @class Base Base.hpp "StreamFormat/Base.hpp"
 */
class Base : public BaseDecode, public BaseEncode {
public:
	struct Exception {
		enum class Code : int {
			InvalidMode = 1,
			InvalidCharacter = 2,
			InvalidFinalBlock = 3
		};
	};//struct Stream::Format::Base::Exception

	Base(BaseMode decMode, BaseMode encMode);

	explicit Base(BaseMode mode);
};//class Stream::Format::Base

void
swap(Base& a, Base& b) noexcept;

std::error_code
make_error_code(Base::Exception::Code e) noexcept;

}//namespace Stream::Format

namespace std {

template <>
struct is_error_code_enum<Stream::Format::Base::Exception::Code> : true_type {};

}//namespace std

#endif //STREAM_FORMAT_BASE_HPP
