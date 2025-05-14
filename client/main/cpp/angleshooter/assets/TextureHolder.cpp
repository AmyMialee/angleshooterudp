#include "PreCompiledClient.h"
#include "TextureHolder.h"

TextureHolder::TextureHolder() : ResourceHolder("textures", std::move(createDefaultTexture())) {
	for (const auto number : NUMBERS) this->load(*number);
	for (const auto letter : ALPHABET) this->load(*letter);
	for (const auto letter : ALPHABET_LOWERCASE) this->load(*letter);
}

unsigned int TextureHolder::getStringWidth(const std::string& string) {
	return static_cast<unsigned int>(string.size() * 28) + 28;
}

sf::Texture TextureHolder::getText(const std::string& string) {
	return getText(string, getStringWidth(string));
}

sf::Texture TextureHolder::getText(const std::string& string, unsigned int width) {
	sf::RenderTexture combinedImage({width, 72});
	auto xOffset = -28;
	for (auto character : string) {
		xOffset += 28;
		const Identifier* id = CHARACTER_MAP[character];
		if (!id) {
			Logger::errorOnce("Character not found in map: " + std::string(1, character));
			continue;
		}
		const sf::Texture* charTexture = getPointer(*id);
		if (!charTexture) {
			Logger::errorOnce("Failed to load texture for character: " + std::string(1, character));
			continue;
		}
		auto charImage = charTexture->copyToImage();
		sf::Sprite charSprite(*charTexture);
		charSprite.setPosition({static_cast<float>(xOffset), 0});
		combinedImage.draw(charSprite);
	}
	auto texture = sf::Texture();
	auto image = combinedImage.getTexture().copyToImage();
	image.flipVertically();
	if (!texture.loadFromImage(image)) {
		Logger::errorOnce("Failed to create texture for string: " + string);
		return getDefault();
	}
	return texture;
}

std::unique_ptr<sf::Texture> TextureHolder::createDefaultTexture() {
	auto texture = std::make_unique<sf::Texture>();
	sf::Image image({16, 16}, sf::Color::Magenta);
	for (unsigned int x = 0; x < 8; ++x) {
		for (unsigned int y = 0; y < 8; ++y) {
			image.setPixel({x, y}, sf::Color::Black);
		}
	}
	for (unsigned int x = 8; x < 16; ++x) {
		for (unsigned int y = 8; y < 16; ++y) {
			image.setPixel({x, y}, sf::Color::Black);
		}
	}
	if (!texture->loadFromImage(image)) Logger::errorOnce("Failed to create missing image texture");
	return texture;
}
