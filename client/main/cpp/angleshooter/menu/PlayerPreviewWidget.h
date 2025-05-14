#pragma once

struct PlayerPreviewWidget : MenuWidget {
	sf::Texture texture;
	sf::Vector2f first;
	sf::Vector2f second;
	int cycleSpeed;
	int cycle = 0;

	explicit PlayerPreviewWidget(sf::Vector2f first = {0, 0}, sf::Vector2f second = {0, 0}, const PlayerCosmetics& cosmetics = PlayerCosmetics(), int cycleSpeed = 20);

	void tick() override;
	void setCosmetics(const PlayerCosmetics& cosmetics);
};
