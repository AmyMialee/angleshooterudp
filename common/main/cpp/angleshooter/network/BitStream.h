#pragma once

class BitStream {
protected:
	uint8_t* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;

	BitStream() : buffer(nullptr), bitHead(0), bitCapacity(0) {}
	BitStream(uint32_t head, uint32_t capacity) : buffer(nullptr), bitHead(head), bitCapacity(capacity) {}
	BitStream(uint8_t* buffer, uint32_t capacity) : buffer(buffer), bitHead(0), bitCapacity(capacity) {}
	BitStream(uint8_t* buffer, uint32_t head, uint32_t capacity) : buffer(buffer), bitHead(head), bitCapacity(capacity) {}
	void reallocBuffer(uint32_t capacity);
public:
	[[nodiscard]] const uint8_t* getBuffer() const;
	[[nodiscard]] uint32_t getBitLength() const;
	[[nodiscard]] uint32_t getByteLength() const;
	[[nodiscard]] uint32_t getRemainingBitCount() const;
};

class OutputBitStream : public BitStream {
public:
	OutputBitStream();
	~OutputBitStream();
	void writeBits(uint8_t data, uint32_t bitCount);
	void writeBits(const void* data, uint32_t bitCount);
	void writeBytes(const void* data, uint32_t bitCount);
	void write(bool data);
	void write(int8_t data);
	void write(uint8_t data);
	void write(char data);
	void write(int16_t data);
	void write(uint16_t data);
	void write(int32_t data);
	void write(uint32_t data);
	void write(float data);
	void write(double data);
	void write(const std::string& string);
	void write(const sf::Vector2f& vector);
	void write(const Identifier& identifier);
	void write(const PlayerCosmetics& cosmetics);
	friend OutputBitStream& operator<<(OutputBitStream& packet, bool data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, int8_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, uint8_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, char data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, int16_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, uint16_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, int32_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, uint32_t data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, float data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, double data);
	friend OutputBitStream& operator<<(OutputBitStream& packet, const std::string& string);
	friend OutputBitStream& operator<<(OutputBitStream& packet, const sf::Vector2f& vector);
	friend OutputBitStream& operator<<(OutputBitStream& packet, const Identifier& identifier);
	friend OutputBitStream& operator<<(OutputBitStream& packet, const PlayerCosmetics& cosmetics);
	template <typename T> void write(T inData, uint32_t inBitCount = sizeof(T) * 8);
};

class InputBitStream : public BitStream {
	bool isOwner;

public:
	InputBitStream(uint8_t* buffer, uint32_t bitCount) : BitStream(buffer, 0, bitCount), isOwner(false) {}
	InputBitStream(const InputBitStream& other);
	~InputBitStream();
	void readBits(uint8_t& output, uint32_t bitCount);
	void readBits(void* output, uint32_t bitCount);
	void readBytes(void* output, uint32_t bitCount);
	void read(bool& output);
	void read(int8_t& output);
	void read(uint8_t& output);
	void read(char& output);
	void read(int16_t& output);
	void read(uint16_t& output);
	void read(int32_t& output);
	void read(uint32_t& output);
	void read(float& output);
	void read(double& output);
	void read(std::string& output);
	void read(sf::Vector2f& output);
	void read(Identifier& output);
	void read(PlayerCosmetics& output);
	friend InputBitStream& operator>>(InputBitStream& packet, bool& output);
	friend InputBitStream& operator>>(InputBitStream& packet, int8_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, uint8_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, char& output);
	friend InputBitStream& operator>>(InputBitStream& packet, int16_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, uint16_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, int32_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, uint32_t& output);
	friend InputBitStream& operator>>(InputBitStream& packet, float& output);
	friend InputBitStream& operator>>(InputBitStream& packet, double& output);
	friend InputBitStream& operator>>(InputBitStream& packet, std::string& output);
	friend InputBitStream& operator>>(InputBitStream& packet, sf::Vector2f& output);
	friend InputBitStream& operator>>(InputBitStream& packet, Identifier& output);
	friend InputBitStream& operator>>(InputBitStream& packet, PlayerCosmetics& output);
	template <typename T> void read(T& inData, uint32_t inBitCount = sizeof(T) * 8);
	void reset(uint32_t inByteCapacity);
};