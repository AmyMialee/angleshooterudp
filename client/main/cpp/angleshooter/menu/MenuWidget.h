#pragma once

struct MenuWidget : sf::RectangleShape {
	explicit MenuWidget(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, const Identifier& texture = Identifier::empty);
	explicit MenuWidget(sf::Vector2f position = {0, 0}, float size = 30, const Identifier& texture = Identifier::empty);

	virtual void tick() {}
};