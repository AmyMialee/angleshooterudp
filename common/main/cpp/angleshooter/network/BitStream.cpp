#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "BitStream.h"

void BitStream::reallocBuffer(uint32_t capacity) {
	if (buffer == nullptr) {
		buffer = static_cast<uint8_t*>(std::malloc(capacity >> 3));
		memset(buffer, 0, capacity >> 3);
	} else {
		const auto tempBuffer = static_cast<uint8_t*>(std::malloc(capacity >> 3));
		memset(tempBuffer, 0, capacity >> 3);
		memcpy(tempBuffer, buffer, bitCapacity >> 3);
		std::free(buffer);
		buffer = tempBuffer;
	}
	bitCapacity = capacity;
}

const uint8_t* BitStream::getBuffer() const {
	return buffer;
}

uint32_t BitStream::getBitLength() const {
	return bitHead;
}

uint32_t BitStream::getByteLength() const {
	return (bitHead + 7) >> 3;
}

uint32_t BitStream::getRemainingBitCount() const {
	return bitCapacity - bitHead;
}

OutputBitStream::OutputBitStream() {
	reallocBuffer(1500 * 8);
}

OutputBitStream::~OutputBitStream() {
	// std::free(buffer);
}

void OutputBitStream::writeBits(uint8_t data, uint32_t bitCount) {
	const auto nextBitHead = bitHead + bitCount;
	if (nextBitHead > bitCapacity) reallocBuffer(std::max(bitCapacity * 2, nextBitHead));
	const auto byteOffset = static_cast<uint8_t>(bitHead >> 3);
	const auto bitOffset = static_cast<uint8_t>(bitHead & 0x7);
	const auto currentMask = static_cast<uint8_t>(~(static_cast<uint8_t>(0xFF) << bitOffset));
	buffer[byteOffset] = static_cast<uint8_t>(buffer[byteOffset] & currentMask) | static_cast<uint8_t>(data << bitOffset);
	if (const auto remainingBits = 8 - bitOffset; std::cmp_less(remainingBits, bitCount)) buffer[byteOffset + 1] = data >> remainingBits;
	bitHead = nextBitHead;
}

void OutputBitStream::writeBits(const void* data, uint32_t bitCount) {
	auto srcByte = static_cast<const char*>(data);
	while (bitCount > 8) {
		writeBits(*srcByte, 8);
		++srcByte;
		bitCount -= 8;
	}
	if (bitCount > 0) writeBits(*srcByte, bitCount);
}

void OutputBitStream::writeBytes(const void* data, uint32_t bitCount) {
	writeBits(data, bitCount << 3);
}

void OutputBitStream::write(bool data) {
	writeBits(&data, 1);
}

void OutputBitStream::write(int8_t data) {
	writeBits(&data, 8);
}

void OutputBitStream::write(uint8_t data) {
	writeBits(&data, 8);
}

void OutputBitStream::write(char data) {
	writeBits(&data, 8);
}

void OutputBitStream::write(int16_t data) {
	writeBits(&data, 16);
}

void OutputBitStream::write(uint16_t data) {
	writeBits(&data, 16);
}

void OutputBitStream::write(int32_t data) {
	writeBits(&data, 32);
}

void OutputBitStream::write(uint32_t data) {
	writeBits(&data, 32);
}

void OutputBitStream::write(float data) {
	writeBits(&data, 32);
}

void OutputBitStream::write(double data) {
	writeBits(&data, 64);
}

void OutputBitStream::write(const std::string& string) {
	write(static_cast<uint32_t>(string.size()));
	for (const auto& element : string) write(element);
}

void OutputBitStream::write(const sf::Vector2f& vector) {
	write(vector.x);
	write(vector.y);
}

void OutputBitStream::write(const Identifier& identifier) {
	write(identifier.getPath());
	write(identifier.getSpace());
}

void OutputBitStream::write(const PlayerCosmetics& cosmetics) {
	write(cosmetics.colour.r);
	write(cosmetics.colour.g);
	write(cosmetics.colour.b);
	write(cosmetics.getCharacterIndex());
	write(cosmetics.getCosmeticIndex());
}

OutputBitStream& operator<<(OutputBitStream& packet, bool data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, int8_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, uint8_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, char data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, int16_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, uint16_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, int32_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, uint32_t data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, float data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, double data) {
	packet.write(data);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, const std::string& string) {
	packet.write(string);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, const sf::Vector2f& vector) {
	packet.write(vector);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, const Identifier& identifier) {
	packet.write(identifier);
	return packet;
}

OutputBitStream& operator<<(OutputBitStream& packet, const PlayerCosmetics& cosmetics) {
	packet.write(cosmetics);
	return packet;
}

template <typename T>
void OutputBitStream::write(T inData, uint32_t inBitCount) {
	static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic Write only supports primitive data types");
	WriteBits(&inData, inBitCount);
}

InputBitStream::InputBitStream(const InputBitStream& other): BitStream(other.bitHead, other.bitCapacity), isOwner(true) {
	const int byteCount = bitCapacity / 8;
	buffer = static_cast<uint8_t*>(malloc(byteCount));
	memcpy(buffer, other.buffer, byteCount);
}

InputBitStream::~InputBitStream() {
	if (isOwner) free(buffer);
}

void InputBitStream::readBits(uint8_t& output, uint32_t bitCount) {
	const auto byteOffset = bitHead >> 3;
	const auto bitOffset = bitHead & 0x7;
	output = buffer[byteOffset] >> bitOffset;
	if (const auto remainingBits = 8 - bitOffset; remainingBits < bitCount) output |= buffer[byteOffset + 1] << remainingBits;
	output &= (~(0x00FF << bitCount));
	bitHead += bitCount;
}

void InputBitStream::readBits(void* output, uint32_t bitCount) {
	auto destByte = static_cast<uint8_t*>(output);
	while (bitCount > 8) {
		readBits(*destByte, 8);
		++destByte;
		bitCount -= 8;
	}
	if (bitCount > 0) readBits(*destByte, bitCount);
}

void InputBitStream::readBytes(void* output, uint32_t bitCount) {
	readBits(output, bitCount << 3);
}

void InputBitStream::read(bool& output) {
	readBits(&output, 1);
}

void InputBitStream::read(int8_t& output) {
	readBits(&output, 8);
}

void InputBitStream::read(uint8_t& output) {
	readBits(&output, 8);
}

void InputBitStream::read(char& output) {
	readBits(&output, 8);
}

void InputBitStream::read(int16_t& output) {
	readBits(&output, 16);
}

void InputBitStream::read(uint16_t& output) {
	readBits(&output, 16);
}

void InputBitStream::read(int32_t& output) {
	readBits(&output, 32);
}

void InputBitStream::read(uint32_t& output) {
	readBits(&output, 32);
}

void InputBitStream::read(float& output) {
	readBits(&output, 32);
}

void InputBitStream::read(double& output) {
	readBits(&output, 64);
}

void InputBitStream::read(std::string& output) {
	uint32_t elementCount;
	read(elementCount);
	output.resize(elementCount);
	for (auto& element : output) read(element);
}

void InputBitStream::read(sf::Vector2f& output) {
	read(output.x);
	read(output.y);
}

void InputBitStream::read(Identifier& output) {
	std::string path;
	std::string space;
	read(path);
	read(space);
	output = Identifier(space, path);
}

void InputBitStream::read(PlayerCosmetics& output) {
	read(output.colour.r);
	read(output.colour.g);
	read(output.colour.b);
	uint8_t characterIndex, cosmeticIndex;
	read(characterIndex);
	read(cosmeticIndex);
	output.character = PlayerCosmetics::getCharacter(characterIndex);
	output.cosmetic = PlayerCosmetics::getCosmetic(cosmeticIndex);
}

InputBitStream& operator>>(InputBitStream& packet, bool& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, int8_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, uint8_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, char& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, int16_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, uint16_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, int32_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, uint32_t& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, float& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, double& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, std::string& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, sf::Vector2f& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, Identifier& output) {
	packet.read(output);
	return packet;
}

InputBitStream& operator>>(InputBitStream& packet, PlayerCosmetics& output) {
	packet.read(output);
	return packet;
}

template <typename T>
void InputBitStream::read(T& inData, uint32_t inBitCount) {
	static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic Read only supports primitive data types");
	ReadBits(&inData, inBitCount);
}

void InputBitStream::reset(uint32_t inByteCapacity) {
	bitCapacity = inByteCapacity << 3;
	bitHead = 0;
}
