#include "PreCompiledClient.h"
#include "TextButton.h"

TextButton::TextButton(sf::Vector2f position, const std::string& texture, int characterWidth, const std::function<void()>& onClick) : MenuButton(position, 0, Identifier::empty, onClick), characterWidth(characterWidth) {
	this->textTexture = getStringTexture(texture, characterWidth);
	this->setTexture(&this->textTexture);
	this->setSize(sf::Vector2f(this->textTexture.getSize()));
}

sf::Texture TextButton::getStringTexture(const std::string& text, int characterWidth) {
	sf::RenderTexture combinedImage({241, 119});
	combinedImage.draw(sf::Sprite(TextureHolder::getInstance().get(Identifier("menu/menu_button_blank.png"))));
	const auto textTexture = TextureHolder::getInstance().getText(text, characterWidth);
	auto textSprite = sf::Sprite(textTexture);
	textSprite.setPosition({241 / 2 - textSprite.getLocalBounds().size.x / 2, 16});
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