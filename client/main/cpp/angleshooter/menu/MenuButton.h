#pragma once

struct MenuButton : MenuWidget {
	std::function<void()> onClick;
	MenuButton* neighbourUp = nullptr;
	MenuButton* neighbourDown = nullptr;
	MenuButton* neighbourLeft = nullptr;
	MenuButton* neighbourRight = nullptr;

	explicit MenuButton(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, const Identifier& texture = Identifier::empty, std::function<void()> onClick = nullptr) : MenuWidget(position, size, texture) {
		if (onClick) this->onClick = std::move(onClick);
	}

	explicit MenuButton(sf::Vector2f position = {0, 0}, float size = 30, const Identifier& texture = Identifier::empty, std::function<void()> onClick = nullptr) : MenuWidget(position, size, texture) {
		if (onClick) this->onClick = std::move(onClick);
	}
};