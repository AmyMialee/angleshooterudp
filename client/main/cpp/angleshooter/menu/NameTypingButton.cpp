#include "PreCompiledClient.h"
#include "NameTypingButton.h"

NameTypingButton::NameTypingButton(sf::Vector2f position, std::string value, const std::function<std::string(std::string)>& textCallback) : MenuButton(position, {0, 0}, Identifier(), [this] {
	this->pressed = true;
	this->inputString = "";
}), textCallback(textCallback), inputString(std::move(value)) {
	underline.setScale({0.6f, 0.6f});
	text.setScale({0.6f, 0.6f});
	underline.setPosition(this->getPosition());
	text.setPosition(this->getPosition() + sf::Vector2f{16, -40} * 0.8f);
	text.rotate(sf::degrees(6));
	underline.setTexture(TextureHolder::getInstance().getPointer(Identifier("menu/menu_options_underline.png")));
	this->refreshTexture();
}

NameTypingButton::NameTypingButton(sf::Vector2f position, const std::string& value, const std::string& textCallback) : NameTypingButton(position, value, createTextFunction(textCallback)) {}

void NameTypingButton::refreshTexture() {
	this->textTexture = TextureHolder::getInstance().getText(this->textCallback ? this->textCallback(this->inputString) : "Slider " + this->inputString);
	text.setSize(sf::Vector2f(this->textTexture.getSize()));
	text.setTexture(&this->textTexture, true);
}

void NameTypingButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(underline);
	target.draw(text);
}

void NameTypingButton::handleEvent(const sf::Event& event) {
	if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
		if (text->unicode >= '!' && text->unicode <= '~' && inputString.size() < 16) {
			inputString += static_cast<char>(text->unicode);
			this->refreshTexture();
		}
		return;
	}
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			this->inputString = OptionsManager::get().getName();
			this->pressed = false;
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace) {
			if (!inputString.empty()) inputString.pop_back();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Enter && inputString.size() >= 1) {
			OptionsManager::get().setName(inputString);
			this->pressed = false;
		}
		this->refreshTexture();
	}
}

std::function<std::string(std::string)> NameTypingButton::createTextFunction(const std::string& textFunction) {
	return [textFunction](const std::string& value) {
		return textFunction + " " + value;
	};
}

NameTypingButton* NameTypingButton::setTextFunction(const std::string& textFunction) {
	this->textCallback = createTextFunction(textFunction);
	return this;
}

NameTypingButton* NameTypingButton::setTextFunction(const std::function<std::string(std::string)>& textFunction) {
	this->textCallback = textFunction;
	return this;
}

sf::Vector2f NameTypingButton::getOffset() const {
	return MenuButton::getOffset() + sf::Vector2f{0, static_cast<float>(this->pressed * 10)};
}