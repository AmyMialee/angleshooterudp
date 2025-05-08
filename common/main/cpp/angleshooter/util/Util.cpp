#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Util.h"

namespace {
	std::default_random_engine createRandomEngine() {
		const auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}
	auto randomEngine = createRandomEngine();
}

sf::Vector2f Util::lerp(float delta, const sf::Vector2f& a, const sf::Vector2f& b) {
	return a + delta * (b - a);
}

double Util::toRadians(double degrees) {
	return degrees * M_PI / 180;
}

double Util::toDegrees(double radians) {
	return radians * 180 / M_PI;
}

void Util::centre(sf::Sprite& sprite) {
	const auto bounds = sprite.getLocalBounds();
	sprite.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2), std::floor(bounds.position.y + bounds.size.y / 2)});
}

void Util::centre(sf::Text& text) {
	const auto bounds = text.getLocalBounds();
	text.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2.f), std::floor(bounds.position.y + bounds.size.y / 2.f)});
}

int Util::randomInt(int exclusiveMax) {
	std::uniform_int_distribution distribution(0, exclusiveMax - 1);
	return distribution(randomEngine);
}

float Util::randomFloat() {
	std::uniform_real_distribution<float> distribution(0, 1);
	return distribution(randomEngine);
}

float Util::randomFloat(float min, float max) {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomEngine);
}

float Util::randomNormalFloat(float scale) {
	std::normal_distribution<float> distribution(0, scale);
	return distribution(randomEngine);
}

std::string Util::toRoundedString(float value, int decimals) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(decimals) << value;
	return stream.str();
}

std::string Util::toRoundedString(double value, int decimals) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(decimals) << value;
	return stream.str();
}

uint16_t Util::swapBytes16(uint16_t inData) {
	return (
		(inData >> 8 & 0xFF) |
		(inData << 8 & 0xFF << 8));
}

uint32_t Util::swapBytes32(uint32_t inData) {
	return (
		(inData >> 24 & 0xFF) |
		(inData >>  8 & 0xFF << 8) |
		(inData <<  8 & 0xFF << 16) |
		(inData << 24 & 0xFF << 24));
}

uint64_t Util::swapBytes64(uint64_t inData) {
	return (
		(inData >> 56 & 0xFF) |
		(inData >> 40 & 0xFF << 8) |
		(inData >> 24 & 0xFF << 16) |
		(inData >>  8 & 0xFF << 24) |
		(inData <<  8 & 0xFF00000000) |
		(inData << 24 & 0xFF00000000 << 8) |
		(inData << 40 & 0xFF00000000 << 16) |
		(inData << 56 & 0xFF00000000 << 24));
}