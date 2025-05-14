#include "PreCompiledClient.h"
#include "TextButton.h"

TextButton::TextButton(sf::Vector2f position, const std::string& texture, int characterWidth, const std::function<void()>& onClick) : MenuButton(position, 0, Identifier::empty, onClick), characterWidth(characterWidth) {
	this->textTexture = getStringTexture(texture, characterWidth);
	this->setTexture(&this->textTexture);
	this->setSize(sf::Vector2f(this->textTexture.getSize()));
	this->setScale(sf::Vector2f{1.f, 1.f} * 1.f);
}

sf::Texture TextButton::getStringTexture(const std::string& text, int characterWidth) {
	sf::RenderTexture combinedImage(sf::Vector2u{241, 119});
	auto border = sf::Sprite(TextureHolder::getInstance().get(Identifier("menu/menu_button_blank.png")));
	border.setScale(sf::Vector2f{1.f, 1.f});
	combinedImage.draw(border);
	const auto textTexture = TextureHolder::getInstance().getText(text, characterWidth);
	auto textSprite = sf::Sprite(textTexture);
	Util::centre(textSprite);
	textSprite.setPosition(sf::Vector2f(combinedImage.getSize()) / 2.f);
	textSprite.setScale(sf::Vector2f{1.f, 1.f} * (characterWidth / 28.f));
	combinedImage.draw(textSprite);
	auto texture = sf::Texture();
	auto image = combinedImage.getTexture().copyToImage();
	image.flipVertically();
	if (!texture.loadFromImage(image)) {
		Logger::errorOnce("Failed to create button texture for text: " + text);
		return texture;
	}
	return texture;
}