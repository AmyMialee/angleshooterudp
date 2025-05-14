#pragma once

struct TextButton : MenuButton {
	sf::Texture textTexture;
	int characterWidth;

	explicit TextButton(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, const std::string& texture = "", int characterWidth = 28, const std::function<void()>& onClick = nullptr);
	explicit TextButton(sf::Vector2f position = {0, 0}, float size = 30, const std::string& texture = "", int characterWidth = 28, const std::function<void()>& onClick = nullptr);

	[[nodiscard]] static sf::Texture getStringTexture(const std::string& text, int characterWidth);
};
