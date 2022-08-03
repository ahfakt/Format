#ifndef STREAM_FORMAT_BASE_HPP
#define	STREAM_FORMAT_BASE_HPP

#include "Stream/Buffer.hpp"
#include <memory>

namespace Stream::Format {

enum class BaseMode : short {
	_ALPHABET2ND		= 1,
	_ALPHABET3RD		= 2,
	_ALPHABET4TH		= 3,
	_ALPHABET_MASK		= 3,
	_NPAD				= 4, // no padding
	_PAD				= 8,
	_OPAD				= _NPAD | _PAD, // optional padding
	//_IGNORE_INVALID 	= 128,
	//_SPLITTED			= 256,
	BASE16				= 16 | _NPAD,
	BASE32				= 32 | _OPAD,
	BASE32_NPAD			= 32 | _NPAD,
	BASE32_HEX			= BASE32 | _ALPHABET2ND,
	BASE32_HEX_NPAD		= BASE32_NPAD | _ALPHABET2ND,
	BASE64				= 64 | _OPAD,
	BASE64_NPAD			= 64 | _NPAD,
	BASE64_URL			= BASE64 | _ALPHABET2ND,
	BASE64_URL_NPAD		= BASE64_NPAD | _ALPHABET2ND,
	BASE64_UTF7			= BASE64_NPAD,
	BASE64_IMAP			= BASE64_NPAD | _ALPHABET3RD,
	//BASE64_PEM		= 64 | _PAD | _SPLITTED, // todo line length 64
	//BASE64_MIME		= 64 | _PAD | _SPLITTED | _IGNORE_INVALID, // todo line length 76
};

/**
 * @brief	Stream::Input %Base decoder
 * @class	BaseDecode Base.hpp "StreamFormat/Base.hpp"
 */
class BaseDecode : public BufferInput {
	std::unique_ptr<unsigned char> mCtx;
	std::unique_ptr<unsigned char> mTempBeg;
	unsigned char* mTempCurr = nullptr;
	unsigned char const* mTempEnd = nullptr;
	bool mFinalizeWhenNoData = true;

	std::size_t
	readBytes(std::byte* dest, std::size_t size) override;

	void
	init(short mode);

public:
	struct Exception : Input::Exception
	{ using Input::Exception::Exception; };

	explicit BaseDecode(BaseMode mode, std::size_t buffInitialSize = 0);

	BaseDecode(BaseMode mode, void const* sourceBuff, std::size_t sourceSize);

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
class BaseEncode : public BufferOutput {
	std::unique_ptr<unsigned char> mCtx;

	std::size_t
	writeBytes(std::byte const* src, std::size_t size) override;

	void
	init(short mode);

public:
	struct Exception : Output::Exception
	{ using Output::Exception::Exception; };

	explicit BaseEncode(BaseMode mode, std::size_t buffInitialSize = 0);

	BaseEncode(BaseMode mode, void* sinkBuff, std::size_t sinkSize);

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

	Base(BaseMode decMode, BaseMode encMode,
			std::size_t decBuffInitialSize = 0, std::size_t encBuffInitialSize = 0);

	Base(BaseMode decMode, BaseMode encMode,
			std::size_t decBuffInitialSize, void* sinkBuff, std::size_t sinkSize);

	Base(BaseMode decMode, BaseMode encMode,
			void const* sourceBuff, std::size_t sourceSize, std::size_t encBuffInitialSize = 0);

	Base(BaseMode decMode, BaseMode encMode,
			void const* sourceBuff, std::size_t sourceSize, void* sinkBuff, std::size_t sinkSize);

	explicit Base(BaseMode mode,
			std::size_t decBuffInitialSize = 0, std::size_t encBuffInitialSize = 0);

	Base(BaseMode mode,
			std::size_t decBuffInitialSize, void* sinkBuff, std::size_t sinkSize);

	Base(BaseMode mode,
			void const* sourceBuff, std::size_t sourceSize, std::size_t encBuffInitialSize = 0);

	Base(BaseMode mode,
			void const* sourceBuff, std::size_t sourceSize, void* sinkBuff, std::size_t sinkSize);
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
