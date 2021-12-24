#include <cassert>
#include <iostream>
#include "Stream/File.h"
#include "StreamFormat/Base.h"
#include "Util.h"

std::vector<std::byte>
testEncode(char const* fileName, Stream::Format::BaseMode mode, int length, int maxChunkLength)
{
	std::vector<std::byte> toEncode = StreamTest::Util::GetRandomBytes<std::chrono::minutes>(length);

	IO::File io(fileName, IO::File::Mode::W);
	Stream::FileOutput file;
	Stream::Format::BaseEncode encode(mode, io.getBlockSize());
	io << file << encode;

	StreamTest::Util::WriteRandomChunks(encode, toEncode,
			std::uniform_int_distribution<int> {1, maxChunkLength});
	return toEncode;
}

std::vector<std::byte>
testDecode(char const* fileName, Stream::Format::BaseMode mode, int length, int maxChunkLength)
{
	std::vector<std::byte> decoded;
	decoded.resize(length);

	IO::File io(fileName, IO::File::Mode::R);
	Stream::FileInput file;
	Stream::Format::BaseDecode decode(mode, io.getBlockSize());
	io >> file >> decode;

	StreamTest::Util::ReadRandomChunks(decode, decoded,
			std::uniform_int_distribution<int> {1, maxChunkLength});
	return decoded;
}

void
test(char const* fileName, Stream::Format::BaseMode mode, int length, int maxChunkLength)
{
	auto encode = testEncode(fileName, mode,  length, maxChunkLength);
	auto decode = testDecode(fileName, mode,  length, maxChunkLength);
	assert(encode == decode);
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());

	int length = 1024*64;
	int maxChunkLength = 256;

	test("base16.enc", Stream::Format::BaseMode::BASE16,  length, maxChunkLength);

	length += std::uniform_int_distribution<int>{1, 5}(gen);
	test("base32.enc", Stream::Format::BaseMode::BASE32,  length, maxChunkLength);
	test("base32_npad.enc", Stream::Format::BaseMode::BASE32_NPAD,  length, maxChunkLength);
	test("base32_hex.enc", Stream::Format::BaseMode::BASE32_HEX,  length, maxChunkLength);
	test("base32_hex_npad.enc", Stream::Format::BaseMode::BASE32_HEX_NPAD,  length, maxChunkLength);

	length += std::uniform_int_distribution<int>{1, 3}(gen);
	test("base64.enc", Stream::Format::BaseMode::BASE64,  length, maxChunkLength);
	test("base64_npad.enc", Stream::Format::BaseMode::BASE64_NPAD,  length, maxChunkLength);
	test("base64_url.enc", Stream::Format::BaseMode::BASE64_URL,  length, maxChunkLength);
	test("base64_url_npad.enc", Stream::Format::BaseMode::BASE64_URL_NPAD,  length, maxChunkLength);
	test("base64_utf7.enc", Stream::Format::BaseMode::BASE64_UTF7,  length, maxChunkLength);
	test("base64_imap.enc", Stream::Format::BaseMode::BASE64_IMAP,  length, maxChunkLength);

	std::cout
			<< "bool "
			<< std::to_string(std::numeric_limits<bool>::max()) << '\n'
			<< "char8_t "
			<< std::to_string(std::numeric_limits<char8_t>::max()) << '\n'
			<< "char "
			<< std::to_string(std::numeric_limits<char>::max()) << '\n'
			<< "unsigned char "
			<< std::to_string(std::numeric_limits<unsigned char>::max()) << '\n'
			<< "char16_t "
			<< std::to_string(std::numeric_limits<char16_t>::max()) << '\n'
			<< "short "
			<< std::to_string(std::numeric_limits<short>::max()) << '\n'
			<< "unsigned short "
			<< std::to_string(std::numeric_limits<unsigned short>::max()) << '\n'
			<< "wchar_t "
			<< std::to_string(std::numeric_limits<wchar_t>::max()) << '\n'
			<< "char32_t "
			<< std::to_string(std::numeric_limits<char32_t>::max()) << '\n'
			<< "int "
			<< std::to_string(std::numeric_limits<int>::max()) << '\n'
			<< "unsigned int "
			<< std::to_string(std::numeric_limits<unsigned int>::max()) << '\n'
			<< "long "
			<< std::to_string(std::numeric_limits<long>::max()) << '\n'
			<< "unsigned long "
			<< std::to_string(std::numeric_limits<unsigned long>::max()) << '\n'
			<< "long long "
			<< std::to_string(std::numeric_limits<long long>::max()) << '\n'
			<< "unsigned long long "
			<< std::to_string(std::numeric_limits<unsigned long long>::max()) << '\n'
			<< "float "
			<< std::to_string(std::numeric_limits<float>::max()) << '\n'
			<< "double "
			<< std::to_string(std::numeric_limits<double>::max()) << '\n'
			<< "long double "
			<< std::to_string(std::numeric_limits<long double>::max()) << '\n';

	return 0;
}

