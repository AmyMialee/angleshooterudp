#pragma once

struct HidingWidget : MenuWidget {
	std::function<bool()> predicate;

	explicit HidingWidget(std::function<bool()> predicate, sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, const Identifier& texture = Identifier::empty);
	explicit HidingWidget(std::function<bool()> predicate, sf::Vector2f position = {0, 0}, float size = 0, const Identifier& texture = Identifier::empty);

	void tick() override;

};
