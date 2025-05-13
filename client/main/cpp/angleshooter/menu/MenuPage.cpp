#include "PreCompiledClient.h"
#include "MenuPage.h"

MenuPage::MenuPage(const sf::View& view, MenuPage* previousPage) : view(view), previousPage(previousPage) {}

void MenuPage::tick() {
	for (const auto& widget : this->widgets) widget->tick();
	for (const auto& button : this->buttons) button->tick(button == this->selectedButton);
}

void MenuPage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const auto& widget : this->widgets) target.draw(*widget, states);
	for (const auto& widget : this->buttons) {
		auto view = target.getView();
		target.setView(sf::View({view.getCenter() + sf::Vector2f{0, static_cast<float>(std::pow(widget->selectedTime, 4) * 20)}, view.getSize()}));
		target.draw(*widget, states);
		target.setView(view);
	}
}

MenuWidget* MenuPage::addWidget(MenuWidget* page) {
	this->widgets.push_back(page);
	return page;
}

MenuButton* MenuPage::addButton(MenuButton* button, MenuButton* neighbour, MenuInput side) {
	this->buttons.push_back(button);
	if (button != nullptr) {
		if (this->selectedButton == nullptr) this->selectedButton = button;
		if (neighbour != nullptr) addLink(button, neighbour, side);
	}
	return button;
}

void MenuPage::addLink(MenuButton* button1, MenuButton* button2, MenuInput side) {
	switch (side) {
		case MenuInput::UP:
			if (button1->neighbourUp == nullptr) button1->neighbourUp = button2;
			if (button2->neighbourDown == nullptr) button2->neighbourDown = button1;
			break;
		case MenuInput::DOWN:
			if (button1->neighbourDown == nullptr) button1->neighbourDown = button2;
			if (button2->neighbourUp == nullptr) button2->neighbourUp = button1;
			break;
		case MenuInput::LEFT:
			if (button1->neighbourLeft == nullptr) button1->neighbourLeft = button2;
			if (button2->neighbourRight == nullptr) button2->neighbourRight = button1;
			break;
		case MenuInput::RIGHT:
			if (button1->neighbourRight == nullptr) button1->neighbourRight = button2;
			if (button2->neighbourLeft == nullptr) button2->neighbourLeft = button1;
			break;
	}
}

void MenuPage::clearButtons() {
	this->buttons.clear();
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
