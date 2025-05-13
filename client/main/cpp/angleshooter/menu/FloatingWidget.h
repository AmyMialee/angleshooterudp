#pragma once

struct FloatingWidget : MenuWidget {
	sf::Vector2f first;
	sf::Vector2f second;
	int cycleSpeed;
	int cycle = 0;

	explicit FloatingWidget(sf::Vector2f first = {0, 0}, sf::Vector2f second = {0, 0}, sf::Vector2f size = {0, 0}, const Identifier& texture = Identifier::empty, int cycleSpeed = 20);
	explicit FloatingWidget(sf::Vector2f first = {0, 0}, sf::Vector2f second = {0, 0}, float size = 30, const Identifier& texture = Identifier::empty, int cycleSpeed = 20);

	void tick() override;
};