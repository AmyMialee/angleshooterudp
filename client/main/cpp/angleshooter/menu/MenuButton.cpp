#include "PreCompiledClient.h"
#include "MenuButton.h"

MenuButton::MenuButton(sf::Vector2f position, sf::Vector2f size, const Identifier& texture, std::function<void()> onClick) : MenuWidget(position, size, texture) {
	if (onClick) this->onClick = std::move(onClick);
}

MenuButton::MenuButton(sf::Vector2f position, float size, const Identifier& texture, std::function<void()> onClick) : MenuWidget(position, size, texture) {
	if (onClick) this->onClick = std::move(onClick);
}

void MenuButton::tick(bool selected) {
	this->selectedTime += selected ? 0.1f : -0.1f;
	this->selectedTime = std::clamp(this->selectedTime, 0.f, 1.f);
}