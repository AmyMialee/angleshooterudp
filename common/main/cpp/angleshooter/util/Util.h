#pragma once

class Util {
public:
	static sf::Vector2f lerp(float delta, const sf::Vector2f& a, const sf::Vector2f& b);
	static sf::View lerp(float delta, const sf::View& a, const sf::View& b);
	static double toRadians(double degrees);
	static double toDegrees(double radians);
	static void centre(sf::Sprite& sprite);
	static void centre(sf::Text& text);
	static void centre(sf::Shape& shape);
	static int randomInt(int exclusiveMax);
	static float randomFloat();
	static float randomFloat(float min, float max);
	static float randomNormalFloat(float scale);
	static std::string toRoundedString(float value, int decimals = 2);
	static std::string toRoundedString(double value, int decimals = 2);
	static uint16_t swapBytes16(uint16_t inData);
	static uint32_t swapBytes32(uint32_t inData);
	static uint64_t swapBytes64(uint64_t inData);
	// Colour Methods Taken From https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a
	static std::tuple<float, float, float> rgbToHsl(const sf::Color& color);
	static std::tuple<float, float, float> rgbToHsv(const sf::Color& color);
	static sf::Color hslToRgb(float hue, float saturation, float lightness);
	static sf::Color hsvToRgb(float hue, float saturation, float value);
};