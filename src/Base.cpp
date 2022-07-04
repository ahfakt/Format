#include "StreamFormat/Base.h"
#include <cstring>
#include <unistd.h>

namespace Stream::Format {

struct BaseContext {
	char const* table = nullptr;
	unsigned char beg[8];
	unsigned char* curr = nullptr;
	unsigned char const* end = nullptr;
	bool (*update)(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl) = nullptr;
	void (*final)(BaseContext* ctx, unsigned char* out, int* outl) = nullptr;
	unsigned short mode = 0;
	unsigned char rawLength = 0;
	unsigned char encodedLength = 0;
};

char const
Base64DecodeTable[][256] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	 52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
	 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	 -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	 41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,
	 52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
	 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,63,
	 -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	 41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,63,-1,-1,-1,
	 52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
	 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	 -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	 41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};

bool
BaseValidate(BaseContext* ctx, unsigned char const* in, int inl)
{
	int r = 0;
	if (ctx->mode & static_cast<short>(BaseMode::_PAD)) {
		r = static_cast<int>(ctx->curr - ctx->beg + inl) & (ctx->encodedLength - 1);
		if (r > 2) r = r - 2;
		else if (r == 0) r = ctx->encodedLength - 2;
		else r = 0;
		if (r > inl)
			r = inl;
	}

	for (int i = 0; i < inl - r; ++i)
		if (ctx->table[in[i]] < 0)
			throw BaseDecode::Exception(Base::Exception::Code::InvalidCharacter, std::string("'") + static_cast<char>(in[i]) + "'");

	for (int i = inl - r; i < inl; ++i) {
		if (ctx->table[in[i]] < 0) {
			if (in[i] == '=')
				return true;
			throw BaseDecode::Exception(Base::Exception::Code::InvalidCharacter, std::string("'") + static_cast<char>(in[i]) + "'");
		}
	}
	return false;
}

bool
Base64DecodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	bool endOfStream = BaseValidate(ctx, reinterpret_cast<unsigned char const*>(in), inl);

	if (ctx->curr != ctx->beg) {
		for (; inl && ctx->curr < ctx->end; ++ctx->curr, ++in, --inl)
			*ctx->curr = *in;
		if (ctx->curr != ctx->end)
			return endOfStream;
		if (endOfStream && !inl)
			return true;
		out[0] = ctx->table[ctx->beg[0]] << 2 | ctx->table[ctx->beg[1]] >> 4;
		out[1] = ctx->table[ctx->beg[1]] << 4 | ctx->table[ctx->beg[2]] >> 2;
		out[2] = ctx->table[ctx->beg[2]] << 6 | ctx->table[ctx->beg[3]];
		out += 3;
		*outl = 3;
		ctx->curr = ctx->beg;
	}

	for(int l = endOfStream ? 5 : 4; inl >= l; in += 4, inl -= 4, out += 3, *outl += 3) {
		out[0] = ctx->table[in[0]] << 2 | ctx->table[in[1]] >> 4;
		out[1] = ctx->table[in[1]] << 4 | ctx->table[in[2]] >> 2;
		out[2] = ctx->table[in[2]] << 6 | ctx->table[in[3]];
	}

	for (; inl; ++ctx->curr, ++in, --inl)
		*ctx->curr = *in;

	return endOfStream;
}

void
Base64DecodeFinal(BaseContext* ctx, unsigned char* out, int* outl)
{
	if (ctx->curr != ctx->beg) {
		if (--ctx->curr != ctx->beg) {
			out[0] = ctx->table[ctx->beg[0]] << 2 | ctx->table[ctx->beg[1]] >> 4;
			if (--ctx->curr != ctx->beg && ctx->beg[2] != '=') {
				out[1] = ctx->table[ctx->beg[1]] << 4 | ctx->table[ctx->beg[2]] >> 2;
				if (--ctx->curr != ctx->beg && ctx->beg[3] != '=') {
					out[2] = ctx->table[ctx->beg[2]] << 6 | ctx->table[ctx->beg[3]];
					*outl = 3;
				} else
					*outl = 2;
			} else
				*outl = 1;
		} else
			throw BaseDecode::Exception(Base::Exception::Code::InvalidFinalBlock);
	} else
		*outl = 0;
}

char const
Base32DecodeTable[][256] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,26,27,28,29,30,31,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
	 -1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
	 25,26,27,28,29,30,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};

bool
Base32DecodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	bool endOfStream = BaseValidate(ctx, in, inl);

	if (ctx->curr != ctx->beg) {
		for (; inl && ctx->curr < ctx->end; ++ctx->curr, ++in, --inl)
			*ctx->curr = *in;
		if (ctx->curr != ctx->end)
			return endOfStream;
		if (endOfStream && !inl)
			return true;
		out[0] = ctx->table[ctx->beg[0]] << 3 | ctx->table[ctx->beg[1]] >> 2;
		out[1] = ctx->table[ctx->beg[1]] << 6 | ctx->table[ctx->beg[2]] << 1 | ctx->table[ctx->beg[3]] >> 4;
		out[2] = ctx->table[ctx->beg[3]] << 4 | ctx->table[ctx->beg[4]] >> 1;
		out[3] = ctx->table[ctx->beg[4]] << 7 | ctx->table[ctx->beg[5]] << 2 | ctx->table[ctx->beg[6]] >> 3;
		out[4] = ctx->table[ctx->beg[6]] << 5 | ctx->table[ctx->beg[7]];
		out += 5;
		*outl = 5;
		ctx->curr = ctx->beg;
	}

	for(int l = endOfStream ? 9 : 8; inl >= l; in += 8, inl -= 8, out += 5, *outl += 5) {
		out[0] = ctx->table[in[0]] << 3 | ctx->table[in[1]] >> 2;
		out[1] = ctx->table[in[1]] << 6 | ctx->table[in[2]] << 1 | ctx->table[in[3]] >> 4;
		out[2] = ctx->table[in[3]] << 4 | ctx->table[in[4]] >> 1;
		out[3] = ctx->table[in[4]] << 7 | ctx->table[in[5]] << 2 | ctx->table[in[6]] >> 3;
		out[4] = ctx->table[in[6]] << 5 | ctx->table[in[7]];
	}

	for (; inl; ++ctx->curr, ++in, --inl)
		*ctx->curr = *in;

	return endOfStream;
}

void
Base32DecodeFinal(BaseContext* ctx, unsigned char* out, int* outl)
{
	if (ctx->curr != ctx->beg) {
		if (--ctx->curr != ctx->beg) {
			out[0] = ctx->table[ctx->beg[0]] << 3 | ctx->table[ctx->beg[1]] >> 2;
			if (--ctx->curr != ctx->beg && ctx->beg[2] != '=') {
				if (--ctx->curr != ctx->beg && ctx->beg[3] != '=') {
					out[1] = ctx->table[ctx->beg[1]] << 6 | ctx->table[ctx->beg[2]] << 1 | ctx->table[ctx->beg[3]] >> 4;
					if (--ctx->curr != ctx->beg && ctx->beg[4] != '=') {
						out[2] = ctx->table[ctx->beg[3]] << 4 | ctx->table[ctx->beg[4]] >> 1;
						if (--ctx->curr != ctx->beg && ctx->beg[5] != '=') {
							if (--ctx->curr != ctx->beg && ctx->beg[6] != '=') {
								out[3] = ctx->table[ctx->beg[4]] << 7 | ctx->table[ctx->beg[5]] << 2 | ctx->table[ctx->beg[6]] >> 3;
								if (--ctx->curr != ctx->beg && ctx->beg[7] != '=') {
									out[4] = ctx->table[ctx->beg[6]] << 5 | ctx->table[ctx->beg[7]];
									*outl = 5;
								} else
									*outl = 4;
							} else
								throw BaseDecode::Exception(Base::Exception::Code::InvalidFinalBlock);
						} else
							*outl = 3;
					} else
						*outl = 2;
				} else
					throw BaseDecode::Exception(Base::Exception::Code::InvalidFinalBlock);
			} else
				*outl = 1;
		} else
			throw BaseDecode::Exception(Base::Exception::Code::InvalidFinalBlock);
	} else
		*outl = 0;
}

char const
Base16DecodeTable[] =
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
	 -1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

bool
Base16DecodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	for (int i = 0; i < inl; ++i)
		if (ctx->table[in[i]] < 0)
			throw BaseDecode::Exception(Base::Exception::Code::InvalidCharacter, std::string("'") + static_cast<char>(in[i]) + "'");

	if (ctx->curr != ctx->beg) {
		out[0] = ctx->table[ctx->beg[0]] << 4 | ctx->table[in[0]];
		++in;
		--inl;
		++out;
		*outl = 1;
		ctx->curr = ctx->beg;
	}

	for(; inl >= 2; in += 2, inl -= 2, ++out, ++*outl)
		out[0] = ctx->table[in[0]] << 4 | ctx->table[in[1]];

	if (inl) {
		*ctx->curr = *in;
		++ctx->curr;
	}

	return false;
}

void
Base16DecodeFinal(BaseContext* ctx, unsigned char* out, int* outl)
{
	if (ctx->curr != ctx->beg) {
		if (--ctx->curr != ctx->beg) {
			out[0] = ctx->table[ctx->beg[0]] << 4 | ctx->table[ctx->beg[1]];
			*outl = 1;
		} else
			throw BaseDecode::Exception(Base::Exception::Code::InvalidFinalBlock);
	} else
		*outl = 0;
}

BaseDecode::BaseDecode(BaseMode mode, std::size_t initialBuffSize)
		: BufferInput(initialBuffSize)
		, mCtx(new unsigned char[sizeof(BaseContext)])
{ init(static_cast<short>(mode)); }

BaseDecode::BaseDecode(BaseMode mode, void const* sourceBuff, std::size_t sourceSize)
		: BufferInput(sourceBuff, sourceSize)
		, mCtx(new unsigned char[sizeof(BaseContext)])
{ init(static_cast<short>(mode)); }

BaseDecode::BaseDecode(BaseDecode&& other) noexcept
{ swap(*this, other); }

void
swap(BaseDecode& a, BaseDecode& b) noexcept
{
	swap(static_cast<BufferInput&>(a), static_cast<BufferInput&>(b));
	std::swap(a.mCtx, b.mCtx);
	std::swap(a.mTempBeg, b.mTempBeg);
	std::swap(a.mTempCurr, b.mTempCurr);
	std::swap(a.mTempEnd, b.mTempEnd);
	std::swap(a.mFinalizeWhenNoData, b.mFinalizeWhenNoData);
}

BaseDecode&
BaseDecode::operator=(BaseDecode&& other) noexcept
{
	static_cast<BufferInput&>(*this) = static_cast<BufferInput&&>(other);
	std::swap(mCtx, other.mCtx);
	std::swap(mTempBeg, other.mTempBeg);
	std::swap(mTempCurr, other.mTempCurr);
	std::swap(mTempEnd, other.mTempEnd);
	std::swap(mFinalizeWhenNoData, other.mFinalizeWhenNoData);
	return *this;
}

void
BaseDecode::init(short mode)
{
	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (mode & 64) {
		mTempBeg.reset(new unsigned char[ctx->rawLength = 3]);
		ctx->table = Base64DecodeTable[mode & static_cast<short>(BaseMode::_ALPHABET_MASK)];
		ctx->update = Base64DecodeUpdate;
		ctx->final = Base64DecodeFinal;
		ctx->end = ctx->beg + (ctx->encodedLength = 4);
	} else if (mode & 32) {
		mTempBeg.reset(new unsigned char[ctx->rawLength = 5]);
		ctx->table = Base32DecodeTable[mode & static_cast<short>(BaseMode::_ALPHABET_MASK)];
		ctx->update = Base32DecodeUpdate;
		ctx->final = Base32DecodeFinal;
		ctx->end = ctx->beg + (ctx->encodedLength = 8);
	} else if (mode & 16) {
		ctx->table = Base16DecodeTable;
		ctx->update = Base16DecodeUpdate;
		ctx->final = Base16DecodeFinal;
		ctx->rawLength = 1;
		ctx->end = ctx->beg + (ctx->encodedLength = 2);
	} else
		throw Exception(Base::Exception::Code::InvalidMode);

	ctx->curr = ctx->beg;
	ctx->mode = mode;
}

std::size_t
BaseDecode::readBytes(std::byte* dest, std::size_t size)
{
	if (mTempCurr != mTempEnd) { // there is decoded data
		if (size > mTempEnd - mTempCurr)
			size = mTempEnd - mTempCurr;
		std::memcpy(dest, mTempCurr, size);
		mTempCurr += size;
		return size;
	}

	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (!ctx->update)
		throw Exception(Input::Exception::Code::Uninitialized);

	if (size >= ctx->rawLength) {
		size /= ctx->rawLength;
		size *= ctx->encodedLength;
	}
	else {
		size = ctx->encodedLength;
		dest = reinterpret_cast<std::byte*>(mTempBeg.get());
	}

	if (mFinalizeWhenNoData) {
		try {
			size = provideData(size);
		} catch (Input::Exception& exc) {
			if (exc.code() == std::make_error_code(static_cast<std::errc>(ENODATA))) {
				finalizeDecoding();
				return 0; // try again to read from mTemp
			}
			throw;
		}
	} else
		size = provideData(size);

	int outl;
	if (ctx->update(ctx, reinterpret_cast<unsigned char*>(dest), &outl,
			reinterpret_cast<unsigned char const*>(mGet), static_cast<int>(size))) {
		mGet += size;
		finalizeDecoding();
		return outl;
	}

	mGet += size;
	if (dest == reinterpret_cast<std::byte*>(mTempBeg.get())) {
		mTempEnd = (mTempCurr = mTempBeg.get()) + outl;
		return 0; // try again to read from mTemp
	}
	return outl;
}

void
BaseDecode::finalizeDecoding()
{
	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (ctx->update) {
		int outl;
		ctx->final(ctx, mTempBeg.get(), &outl);
		mTempEnd = (mTempCurr = mTempBeg.get()) + outl;
		ctx->update = nullptr;
	}
}

void
BaseDecode::finalizeDecodingWhenNoData(bool on)
{ mFinalizeWhenNoData = on; }

/**
 * @see		<a href="https://tools.ietf.org/html/rfc4648#section-3.4">Choosing the Alphabet</a>
 */
char const
Base64EncodeTable[][65] = {
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,"
};

/**
 * @see		<a href="https://tools.ietf.org/html/rfc4648#section-4">Base 64 Encoding</a>
 */
bool
Base64EncodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	if (ctx->curr != ctx->beg) {
		for (; inl && ctx->curr < ctx->end; ++ctx->curr, ++in, --inl)
			*ctx->curr = *in;
		if (ctx->curr != ctx->end)
			return false;
		out[0] = ctx->table[ctx->beg[0] >> 2];
		out[1] = ctx->table[((ctx->beg[0] & 0x03) << 4) | (ctx->beg[1] >> 4)];
		out[2] = ctx->table[((ctx->beg[1] & 0x0F) << 2) | (ctx->beg[2] >> 6)];
		out[3] = ctx->table[ctx->beg[2] & 0x3F];
		out += 4;
		*outl = 4;
		ctx->curr = ctx->beg;
	}

	for(; inl >= 3; in += 3, inl -= 3, out += 4, *outl += 4) {
		out[0] = ctx->table[in[0] >> 2];
		out[1] = ctx->table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		out[2] = ctx->table[((in[1] & 0x0F) << 2) | (in[2] >> 6)];
		out[3] = ctx->table[in[2] & 0x3F];
	}

	for (; inl; ++ctx->curr, ++in, --inl)
		*ctx->curr = *in;
	return false;
}

/**
 * @see		<a href="https://tools.ietf.org/html/rfc4648#section-3.2">Padding of Encoded Data</a>
 */
void
Base64EncodeFinal(BaseContext* ctx, unsigned char* out, int* outl)
{
	*outl = 0;
	if (ctx->curr != ctx->beg) {
		out[0] = ctx->table[ctx->beg[0] >> 2];
		if (--ctx->curr == ctx->beg) {
			out[1] = ctx->table[(ctx->beg[0] & 0x03) << 4];
			*outl = 2;
		} else {
			out[1] = ctx->table[((ctx->beg[0] & 0x03) << 4) | (ctx->beg[1] >> 4)];
			out[2] = ctx->table[((ctx->beg[1] & 0x0F) << 2)];
			*outl = 3;
		}

		if (ctx->mode & static_cast<short>(BaseMode::_PAD))
			for (out += *outl; *outl < ctx->encodedLength; ++out, ++*outl)
				*out = '=';
	}
}

char const
Base32EncodeTable[][33] {
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567",
		"0123456789ABCDEFGHIJKLMNOPQRSTUV"
};

bool
Base32EncodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	if (ctx->curr != ctx->beg) {
		for (; inl && ctx->curr < ctx->end; ++ctx->curr, ++in, --inl)
			*ctx->curr = *in;
		if (ctx->curr != ctx->end)
			return false;
		out[0] = ctx->table[ctx->beg[0] >> 3];
		out[1] = ctx->table[((ctx->beg[0] & 0x07) << 2) | (ctx->beg[1] >> 6)];
		out[2] = ctx->table[(ctx->beg[1] & 0x3E) >> 1];
		out[3] = ctx->table[((ctx->beg[1] & 0x01) << 4) | (ctx->beg[2] >> 4)];
		out[4] = ctx->table[((ctx->beg[2] & 0x0F) << 1) | (ctx->beg[3] >> 7)];
		out[5] = ctx->table[(ctx->beg[3] & 0x7C) >> 2];
		out[6] = ctx->table[((ctx->beg[3] & 0x03) << 3) | (ctx->beg[4] >> 5)];
		out[7] = ctx->table[ctx->beg[4] & 0x1F];
		out += 8;
		*outl = 8;
		ctx->curr = ctx->beg;
	}

	for(; inl >= 5; in += 5, inl -= 5, out += 8, *outl += 8) {
		out[0] = ctx->table[in[0] >> 3];
		out[1] = ctx->table[((in[0] & 0x07) << 2) | (in[1] >> 6)];
		out[2] = ctx->table[(in[1] & 0x3E) >> 1];
		out[3] = ctx->table[((in[1] & 0x01) << 4) | (in[2] >> 4)];
		out[4] = ctx->table[((in[2] & 0x0F) << 1) | (in[3] >> 7)];
		out[5] = ctx->table[(in[3] & 0x7C) >> 2];
		out[6] = ctx->table[((in[3] & 0x03) << 3) | (in[4] >> 5)];
		out[7] = ctx->table[in[4] & 0x1F];
	}

	for (; inl; ++ctx->curr, ++in, --inl)
		*ctx->curr = *in;
	return false;
}

void
Base32EncodeFinal(BaseContext* ctx, unsigned char* out, int* outl)
{
	*outl = 0;
	if (ctx->curr != ctx->beg) {
		out[0] = ctx->table[ctx->beg[0] >> 3];
		if (--ctx->curr == ctx->beg) {
			out[1] = ctx->table[(ctx->beg[0] & 0x07) << 2];
			*outl = 2;
		} else {
			out[1] = ctx->table[((ctx->beg[0] & 0x07) << 2) | (ctx->beg[1] >> 6)];
			out[2] = ctx->table[(ctx->beg[1] & 0x3E) >> 1];
			if (--ctx->curr == ctx->beg) {
				out[3] = ctx->table[(ctx->beg[1] & 0x01) << 4];
				*outl = 4;
			} else {
				out[3] = ctx->table[((ctx->beg[1] & 0x01) << 4) | (ctx->beg[2] >> 4)];
				if (--ctx->curr == ctx->beg) {
					out[4] = ctx->table[(ctx->beg[2] & 0x0F) << 1];
					*outl = 5;
				} else {
					out[4] = ctx->table[((ctx->beg[2] & 0x0F) << 1) | (ctx->beg[3] >> 7)];
					out[5] = ctx->table[(ctx->beg[3] & 0x7C) >> 2];
					out[6] = ctx->table[(ctx->beg[3] & 0x03) << 3];
					*outl = 7;
				}
			}
		}

		if (ctx->mode & static_cast<short>(BaseMode::_PAD))
			for (out += *outl; *outl < ctx->encodedLength; ++out, ++*outl)
				*out = '=';
	}
}

char const
Base16EncodeTable[] = "0123456789ABCDEF";

bool
Base16EncodeUpdate(BaseContext* ctx, unsigned char* out, int* outl, unsigned char const* in, int inl)
{
	*outl = 0;
	for(; inl; ++in, --inl, out += 2, *outl += 2) {
		out[0] = ctx->table[in[0] >> 4];
		out[1] = ctx->table[in[0] & 0x0F];
	}
	return false;
}

BaseEncode::BaseEncode(BaseMode mode, std::size_t initialBuffSize)
		: BufferOutput(initialBuffSize)
		, mCtx(new unsigned char[sizeof(BaseContext)])
{ init(static_cast<short>(mode)); }

BaseEncode::BaseEncode(BaseMode mode, void* sinkBuff, std::size_t sinkSize)
		: BufferOutput(sinkBuff, sinkSize)
		, mCtx(new unsigned char[sizeof(BaseContext)])
{ init(static_cast<short>(mode)); }

BaseEncode::BaseEncode(BaseEncode&& other) noexcept
{ swap(*this, other); }

void
swap(BaseEncode& a, BaseEncode& b) noexcept
{
	swap(static_cast<BufferOutput&>(a), static_cast<BufferOutput&>(b));
	std::swap(a.mCtx, b.mCtx);
}

BaseEncode&
BaseEncode::operator=(BaseEncode&& other) noexcept
{
	static_cast<BufferOutput&>(*this) = static_cast<BufferOutput&&>(other);
	std::swap(mCtx, other.mCtx);
	return *this;
}

void
BaseEncode::init(short mode)
{
	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (mode & 64) {
		ctx->table = Base64EncodeTable[mode & static_cast<short>(BaseMode::_ALPHABET_MASK)];
		ctx->update = Base64EncodeUpdate;
		ctx->final = Base64EncodeFinal;
		ctx->encodedLength = 4;
		ctx->end = ctx->beg + (ctx->rawLength = 3);
	} else if (mode & 32) {
		ctx->table = Base32EncodeTable[mode & static_cast<short>(BaseMode::_ALPHABET_MASK)];
		ctx->update = Base32EncodeUpdate;
		ctx->final = Base32EncodeFinal;
		ctx->encodedLength = 8;
		ctx->end = ctx->beg + (ctx->rawLength = 5);
	} else if (mode & 16) {
		ctx->table = Base16EncodeTable;
		ctx->update = Base16EncodeUpdate;
		ctx->final = nullptr;
		ctx->encodedLength = 2;
		ctx->rawLength = 1;
		ctx->end = ctx->beg;
	} else
		throw Exception(Base::Exception::Code::InvalidMode);

	ctx->curr = ctx->beg;
	ctx->mode = mode;
}

std::size_t
BaseEncode::writeBytes(std::byte const* src, std::size_t size)
{
	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (!ctx->update)
		throw Exception(Output::Exception::Code::Uninitialized);

	provideSpace(((size - 1) / ctx->rawLength + 1) * ctx->encodedLength);

	int outl;
	ctx->update(ctx, reinterpret_cast<unsigned char*>(mPut), &outl,
			reinterpret_cast<unsigned char const*>(src), static_cast<int>(size));
	mPut += outl;
	return size;
}

void
BaseEncode::finalizeEncoding()
{
	auto* ctx = reinterpret_cast<BaseContext*>(mCtx.get());
	if (ctx->update) {
		if (ctx->final) {
			provideSpace(ctx->encodedLength);
			int outl;
			ctx->final(ctx, reinterpret_cast<unsigned char*>(mPut), &outl);
			mPut += outl;
		}
		ctx->update = nullptr;
	}
}

BaseEncode::~BaseEncode()
{
	try {
		finalizeEncoding();
	} catch (Output::Exception& exc) {
		::write(STDERR_FILENO, exc.what(), std::strlen(exc.what()));
	}
}

Base::Base(BaseMode decMode, BaseMode encMode,
		std::size_t decBuffInitialSize, std::size_t encBuffInitialSize)
		: BaseDecode(decMode, decBuffInitialSize)
		, BaseEncode(encMode, encBuffInitialSize)
{}

Base::Base(BaseMode decMode, BaseMode encMode,
		std::size_t decBuffInitialSize, void* sinkBuff, std::size_t sinkSize)
		: BaseDecode(decMode, decBuffInitialSize)
		, BaseEncode(encMode, sinkBuff, sinkSize)
{}

Base::Base(BaseMode decMode, BaseMode encMode,
		void const* sourceBuff, std::size_t sourceSize, std::size_t encBuffInitialSize)
		: BaseDecode(decMode, sourceBuff, sourceSize)
		, BaseEncode(encMode, encBuffInitialSize)
{}

Base::Base(BaseMode decMode, BaseMode encMode,
		void const* sourceBuff, std::size_t sourceSize, void* sinkBuff, std::size_t sinkSize)
		: BaseDecode(decMode, sourceBuff, sourceSize)
		, BaseEncode(encMode, sinkBuff, sinkSize)
{}

Base::Base(BaseMode mode,
		std::size_t decBuffInitialSize, std::size_t encBuffInitialSize)
		: Base(mode, mode, decBuffInitialSize, encBuffInitialSize)
{}

Base::Base(BaseMode mode,
		std::size_t decBuffInitialSize, void* sinkBuff, std::size_t sinkSize)
		: Base(mode, mode, decBuffInitialSize, sinkBuff, sinkSize)
{}

Base::Base(BaseMode mode,
		void const* sourceBuff, std::size_t sourceSize, std::size_t encBuffInitialSize)
		: Base(mode, mode, sourceBuff, sourceSize, encBuffInitialSize)
{}

Base::Base(BaseMode mode,
		void const* sourceBuff, std::size_t sourceSize, void* sinkBuff, std::size_t sinkSize)
		: Base(mode, mode, sourceBuff, sourceSize, sinkBuff, sinkSize)
{}

void
swap(Base& a, Base& b) noexcept
{
	swap(static_cast<BaseDecode&>(a), static_cast<BaseDecode&>(b));
	swap(static_cast<BaseEncode&>(a), static_cast<BaseEncode&>(b));
}

std::error_code
make_error_code(Base::Exception::Code e) noexcept
{
	static struct : std::error_category {
		[[nodiscard]] char const*
		name() const noexcept override
		{ return "Stream::Format::Base"; }

		[[nodiscard]] std::string
		message(int e) const noexcept override
		{
			switch (e) {
			case 1: return "Invalid Mode";
			case 2: return "Invalid Character";
			case 3: return "Invalid Final Block";
			default: return "Unknown Error";
			}
		}
	} instance;
	return {static_cast<int>(e), instance};
}

}//namespace Stream::Format
