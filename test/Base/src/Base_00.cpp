#include <cassert>
#include <iostream>
#include <Stream/File.hpp>
#include <Format/Base.hpp>
#include <Stream/Test/Util.hpp>

std::vector<std::byte>
testEncode(char const* fileName, Format::BaseMode mode, int length, int maxChunkLength)
{
	std::vector<std::byte> toEncode = Stream::Test::GetRandomBytes<std::chrono::minutes>(length);

	Stream::File file(fileName, Stream::File::Mode::W);
	Stream::BufferOutput buffer(file.getBlockSize());
	Format::BaseEncode encoder(mode);
	file < buffer < encoder;

	Stream::Test::WriteRandomChunks(encoder, toEncode,
			std::uniform_int_distribution<int> {1, maxChunkLength});
	return toEncode;
}

std::vector<std::byte>
testDecode(char const* fileName, Format::BaseMode mode, int length, int maxChunkLength)
{
	std::vector<std::byte> decoded;
	decoded.resize(length);

	Stream::File file(fileName, Stream::File::Mode::R);
	Stream::BufferInput buffer(file.getBlockSize());
	Format::BaseDecode decoder(mode);
	file > buffer > decoder;

	Stream::Test::ReadRandomChunks(decoder, decoded,
			std::uniform_int_distribution<int> {1, maxChunkLength});
	return decoded;
}

void
test(char const* fileName, Format::BaseMode mode, int length, int maxChunkLength)
{
	auto encode = testEncode(fileName, mode, length, maxChunkLength);
	auto decode = testDecode(fileName, mode, length, maxChunkLength);
	assert(encode == decode);
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());

	int length = 1024*64;
	int maxChunkLength = 256;

	test("base16.enc", Format::BaseMode::BASE16, length, maxChunkLength);

	length += std::uniform_int_distribution<int>{1, 5}(gen);
	test("base32.enc", Format::BaseMode::BASE32, length, maxChunkLength);
	test("base32_npad.enc", Format::BaseMode::BASE32_NPAD, length, maxChunkLength);
	test("base32_hex.enc", Format::BaseMode::BASE32_HEX, length, maxChunkLength);
	test("base32_hex_npad.enc", Format::BaseMode::BASE32_HEX_NPAD, length, maxChunkLength);

	length += std::uniform_int_distribution<int>{1, 3}(gen);
	test("base64.enc", Format::BaseMode::BASE64, length, maxChunkLength);
	test("base64_npad.enc", Format::BaseMode::BASE64_NPAD, length, maxChunkLength);
	test("base64_url.enc", Format::BaseMode::BASE64_URL, length, maxChunkLength);
	test("base64_url_npad.enc", Format::BaseMode::BASE64_URL_NPAD, length, maxChunkLength);
	test("base64_utf7.enc", Format::BaseMode::BASE64_UTF7, length, maxChunkLength);
	test("base64_imap.enc", Format::BaseMode::BASE64_IMAP, length, maxChunkLength);

	return 0;
}

