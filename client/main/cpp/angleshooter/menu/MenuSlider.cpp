#include "PreCompiledClient.h"
#include "MenuSlider.h"

MenuSlider::MenuSlider(sf::Vector2f position, double value, const std::function<std::string(double)>& textCallback, const std::function<void(double)>& finalCallback, const std::function<void(double)>& constantCallback) : MenuButton(position, {0, 0}, Identifier(), [this] {
	this->pressed = true;
}), textCallback(textCallback), constantCallback(constantCallback), finalCallback(finalCallback), value(value) {
	empty.setScale({0.8f, 0.8f});
	bar.setScale({0.8f, 0.8f});
	text.setScale({0.8f, 0.8f});
	empty.setPosition(this->getPosition());
	bar.setPosition(this->getPosition());
	text.setPosition(this->getPosition() + sf::Vector2f{16, -52} * 0.8f);
	text.rotate(sf::degrees(6));
	empty.setTexture(TextureHolder::getInstance().getPointer(Identifier("menu/menu_options_bar_empty.png")));
	bar.setTexture(TextureHolder::getInstance().getPointer(Identifier("menu/menu_options_bar_fill.png")));
	this->refreshTexture();
}

MenuSlider::MenuSlider(sf::Vector2f position, double value, const std::string& textCallback, const std::function<void(double)>& finalCallback, const std::function<void(double)>& constantCallback) : MenuSlider(position, value, createTextFunction(textCallback), finalCallback, constantCallback) {}

void MenuSlider::refreshTexture() {
	bar.setTextureRect({{0, 0}, {static_cast<int>(428 * this->value), 116}});
	bar.setSize({static_cast<float>(428 * this->value), 116});
	if (this->textCallback == nullptr) {
		this->textTexture = TextureHolder::getInstance().getText("Slider " + Util::toRoundedString(this->value * 100, 0));
	} else {
		this->textTexture = TextureHolder::getInstance().getText(this->textCallback(this->value));
	}
	text.setSize(sf::Vector2f(this->textTexture.getSize()));
	text.setTexture(&this->textTexture, true);
}

void MenuSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(empty);
	target.draw(bar);
	target.draw(text);
}

void MenuSlider::tick(bool selected) {
	MenuButton::tick(selected);
	if (forward || backward) {
		this->value += static_cast<float>(forward - backward) * 0.01;
		this->value = std::clamp(value, 0., 1.);
		if (this->constantCallback) this->constantCallback(value);
		this->refreshTexture();
	}
}

void MenuSlider::input(MenuInput input) {
	switch (input) {
		case MenuInput::PRESS:
			this->pressed = false;
			this->forward = false;
			this->backward = false;
			if (this->finalCallback) this->finalCallback(this->value);
			return;
		case MenuInput::LEFT:
			this->backward = true;
			return;
		case MenuInput::RIGHT:
			this->forward = true;
			return;
		case MenuInput::RELEASE_LEFT:
			this->backward = false;
			return;
		case MenuInput::RELEASE_RIGHT:
			this->forward = false;
	}
}

std::function<std::string(double)> MenuSlider::createTextFunction(const std::string& textFunction) {
	return [textFunction](double value) {
		return textFunction + " " + Util::toRoundedString(value * 100, 0);
	};
}

MenuSlider* MenuSlider::setTextFunction(const std::string& textFunction) {
	this->textCallback = createTextFunction(textFunction);
	return this;
}

MenuSlider* MenuSlider::setTextFunction(const std::function<std::string(double)>& textFunction) {
	this->textCallback = textFunction;
	return this;
}

MenuSlider* MenuSlider::setConstantCallback(const std::function<void(double)>& textCallback) {
	this->constantCallback = textCallback;
	return this;
}

MenuSlider* MenuSlider::setFinalCallback(const std::function<void(double)>& callback) {
	this->finalCallback = callback;
	return this;
}

sf::Vector2f MenuSlider::getOffset() const {
	return MenuButton::getOffset() + sf::Vector2f{0, static_cast<float>(this->pressed * 10)};
}