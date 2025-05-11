#include "PreCompiledClient.h"
#include "MenuPage.h"

MenuPage::MenuPage(const sf::View& view, MenuPage* previousPage) : view(view), previousPage(previousPage) {}

void MenuPage::tick() {
	for (const auto& widget : this->buttons) widget->tick();
}

void MenuPage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const auto& widget : this->buttons) {
		auto view = target.getView();
		if (widget == this->selectedButton) {
			auto newView = sf::View({view.getCenter() + sf::Vector2f{0.f, 50.f}, view.getSize()});
			target.setView(newView);
		}
		target.draw(*widget, states);
		target.setView(view);
	}
}

MenuButton* MenuPage::addButton(MenuButton* button, MenuButton* neighbour, MenuInput side) {
	this->buttons.push_back(button);
	if (button != nullptr) {
		if (this->selectedButton == nullptr) this->selectedButton = button;
		if (neighbour != nullptr) switch (side) {
			case MenuInput::UP:
				if (button->neighbourUp != nullptr) Logger::error("MenuPage::addWidget: neighbourUp already set");
				button->neighbourUp = neighbour;
				if (neighbour->neighbourDown != nullptr) Logger::error("MenuPage::addWidget: neighbourDown already set");
				neighbour->neighbourDown = button;
				break;
			case MenuInput::DOWN:
				if (button->neighbourDown != nullptr) Logger::error("MenuPage::addWidget: neighbourDown already set");
				button->neighbourDown = neighbour;
				if (neighbour->neighbourUp != nullptr) Logger::error("MenuPage::addWidget: neighbourUp already set");
				neighbour->neighbourUp = button;
				break;
			case MenuInput::LEFT:
				if (button->neighbourLeft != nullptr) Logger::error("MenuPage::addWidget: neighbourLeft already set");
				button->neighbourLeft = neighbour;
				if (neighbour->neighbourRight != nullptr) Logger::error("MenuPage::addWidget: neighbourRight already set");
				neighbour->neighbourRight = button;
				break;
			case MenuInput::RIGHT:
				if (button->neighbourRight != nullptr) Logger::error("MenuPage::addWidget: neighbourRight already set");
				button->neighbourRight = neighbour;
				if (neighbour->neighbourLeft != nullptr) Logger::error("MenuPage::addWidget: neighbourLeft already set");
				neighbour->neighbourLeft = button;
				break;
		}
	}
	return button;
}

void MenuPage::input(MenuInput input) {
	if (this->selectedButton == nullptr) return;
	switch (input) {
		case MenuInput::PRESS:
			if (this->selectedButton->onClick) this->selectedButton->onClick();
			break;
		case MenuInput::UP:
			if (this->selectedButton->neighbourUp != nullptr) this->selectedButton = this->selectedButton->neighbourUp;
			break;
		case MenuInput::DOWN:
			if (this->selectedButton->neighbourDown != nullptr) this->selectedButton = this->selectedButton->neighbourDown;
			break;
		case MenuInput::LEFT:
			if (this->selectedButton->neighbourLeft != nullptr) this->selectedButton = this->selectedButton->neighbourLeft;
			break;
		case MenuInput::RIGHT:
			if (this->selectedButton->neighbourRight != nullptr) this->selectedButton = this->selectedButton->neighbourRight;
			break;
	}
}

MenuButton* MenuPage::getSelectedButton() const {
	return this->selectedButton;
}

MenuPage* MenuPage::getPreviousPage() const {
	return this->previousPage;
}

sf::View MenuPage::getView() const {
	return this->view;
}
