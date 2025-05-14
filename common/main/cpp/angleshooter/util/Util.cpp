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

sf::View Util::lerp(float delta, const sf::View& a, const sf::View& b) {
	return {lerp(delta, a.getCenter(), b.getCenter()), lerp(delta, a.getSize(), b.getSize())};
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

void Util::centre(sf::Shape& shape) {
	const auto bounds = shape.getLocalBounds();
	shape.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2.f), std::floor(bounds.position.y + bounds.size.y / 2.f)});
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

// Colour Methods Taken From https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a

std::tuple<float, float, float> Util::rgbToHsl(const sf::Color& color) {
	auto r = color.r / 255.f;
	auto g = color.g / 255.f;
	auto b = color.b / 255.f;
	const auto max = std::max({ r, g, b });
	const auto min = std::min({ r, g, b });
	const auto chroma = max - min; // Chroma
	auto hue = 0.f;
	auto saturation = 0.f;
	auto lightness = 0.f;
	if (chroma != 0.f) {
		if (max == r) {
			hue = std::fmod(((g - b) / chroma), 6.f);
		} else if (max == g) {
			hue = ((b - r) / chroma) + 2;
		} else if (max == b) {
			hue = ((r - g) / chroma) + 4;
		}
		hue *= 60;
	}
	if (hue < 0.f) hue += 360;
	lightness += (max + min) / 2;
	if (lightness != 1.f && lightness != 0.f) saturation = chroma / (1 - std::fabs(2 * lightness - 1));
	return { hue, saturation, lightness };
}

std::tuple<float, float, float> Util::rgbToHsv(const sf::Color& color) {
	auto r = color.r / 255.f;
	auto g = color.g / 255.f;
	auto b = color.b / 255.f;
	const auto max = std::max({ r, g, b });
	const auto min = std::min({ r, g, b });
	const auto chroma = max - min;
	auto hue = 0.f;
	auto saturation = 0.f;
	if (chroma != 0.f) {
		if (max == r) {
			hue = std::fmod(((g - b) / chroma), 6.f);
		} else if (max == g) {
			hue = ((b - r) / chroma) + 2;
		} else if (max == b) {
			hue = ((r - g) / chroma) + 4;
		}
		hue *= 60;
	}
	if (hue < 0.f)hue += 360;
	auto value = max;
	if (value != 0.f)saturation = chroma / value;
	return { hue, saturation, value };
}

sf::Color Util::hslToRgb(float hue, float saturation, float lightness) {
	const auto chroma = (1 - std::fabs(2 * lightness - 1)) * saturation;
	const auto hPrime = hue / 60; // H'
	const auto x = chroma * (1 - std::fabs(std::fmod(hPrime, 2.f) - 1));
	const auto m = lightness - chroma / 2;
	auto r = 0.f;
	auto g = 0.f;
	auto b = 0.f;
	switch (static_cast<int>(hPrime)) {
		case 0: r = chroma; g = x;        break;
		case 1: r = x; g = chroma;        break;
		case 2:        g = chroma; b = x; break;
		case 3:        g = x; b = chroma; break;
		case 4: r = x;        b = chroma; break;
		case 5: r = chroma;        b = x; break;
	}
	r += m;
	g += m;
	b += m;
	sf::Color color;
	color.r = static_cast<uint8_t>(std::round(r * 255));
	color.g = static_cast<uint8_t>(std::round(g * 255));
	color.b = static_cast<uint8_t>(std::round(b * 255));
	return color;
}

sf::Color Util::hsvToRgb(float hue, float saturation, float value) {
	const auto chroma = saturation * value;
	const auto hPrime = std::fmod(hue / 60, 6.f);
	const auto x = chroma * (1 - std::fabs(std::fmod(hPrime, 2.f) - 1));
	const auto m = value - chroma;
	auto r = 0.f;
	auto g = 0.f;
	auto b = 0.f;
	switch (static_cast<int>(hPrime)) {
		case 0: r = chroma; g = x;        break;
		case 1: r = x; g = chroma;        break;
		case 2:        g = chroma; b = x; break;
		case 3:        g = x; b = chroma; break;
		case 4: r = x;        b = chroma; break;
		case 5: r = chroma;        b = x; break;
	}
	r += m;
	g += m;
	b += m;
	sf::Color color;
	color.r = static_cast<uint8_t>(std::round(r * 255));
	color.g = static_cast<uint8_t>(std::round(g * 255));
	color.b = static_cast<uint8_t>(std::round(b * 255));
	return color;
}