#include "PreCompiledClient.h"
#include "DirectConnectButton.h"

DirectConnectButton::DirectConnectButton(sf::Vector2f position, const std::string& value) : MenuButton(position, 0, Identifier::empty, [this] {
	this->pressed = true;
	this->inputString = "";
}), inputString(value), boxName(value) {
	this->refreshTexture();
}

void DirectConnectButton::refreshTexture() {
	sf::RenderTexture combinedImage(sf::Vector2u{241, 119});
	auto border = sf::Sprite(TextureHolder::getInstance().get(Identifier("menu/menu_button_blank.png")));
	border.setScale(sf::Vector2f{1.f, 1.f});
	combinedImage.draw(border);
	const auto characterWidth = 18;
	const auto textTexture = TextureHolder::getInstance().getText(inputString, characterWidth);
	auto textSprite = sf::Sprite(textTexture);
	Util::centre(textSprite);
	textSprite.setPosition(sf::Vector2f(combinedImage.getSize()) / 2.f);
	textSprite.setScale(sf::Vector2f{1.f, 1.f} * (characterWidth / 28.f));
	combinedImage.draw(textSprite);
	this->textTexture = sf::Texture();
	auto image = combinedImage.getTexture().copyToImage();
	image.flipVertically();
	if (!this->textTexture.loadFromImage(image)) Logger::errorOnce("Failed to create button texture for text: " + inputString);
	this->setSize(sf::Vector2f(this->textTexture.getSize()));
	this->setTexture(&this->textTexture, true);
}

void DirectConnectButton::handleEvent(const sf::Event& event) {
	if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
		if (((text->unicode >= '0' && text->unicode <= '9') || text->unicode == '.') && inputString.size() < 16) {
			inputString += static_cast<char>(text->unicode);
			this->refreshTexture();
		}
		return;
	}
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			this->inputString = boxName;
			this->pressed = false;
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace) {
			if (!inputString.empty()) inputString.pop_back();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Enter && inputString.size() >= 7) {
			if (const auto ip = sf::IpAddress::resolve(this->inputString); ip.has_value()) {
				const auto pip = PortedIP{.ip = ip.value(), .port = AngleShooterCommon::PORT};
				AngleShooterClient::get().connect(pip);
			}
			this->pressed = false;
		}
		this->refreshTexture();
	}
}

sf::Vector2f DirectConnectButton::getOffset() const {
	return MenuButton::getOffset() + sf::Vector2f{0, static_cast<float>(this->pressed * 10)};
}