#pragma once

struct DirectConnectButton : MenuButton {
	bool pressed = false;
	sf::Texture textTexture;
	std::string inputString;
	std::string boxName;

	explicit DirectConnectButton(sf::Vector2f position = {0, 0}, const std::string& value = "");
	void refreshTexture();
	void handleEvent(const sf::Event& event);
	[[nodiscard]] sf::Vector2f getOffset() const override;
};