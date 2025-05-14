#include "PreCompiledClient.h"
#include "PlayerPreviewWidget.h"

PlayerPreviewWidget::PlayerPreviewWidget(sf::Vector2f first, sf::Vector2f second, const PlayerCosmetics& cosmetics, int cycleSpeed) : MenuWidget(first, {128, 128}) {
	this->setCosmetics(cosmetics);
	this->first = first;
	this->second = second;
	this->cycleSpeed = cycleSpeed;
}

void PlayerPreviewWidget::tick() {
	MenuWidget::tick();
	if (cycleSpeed > 0) {
		cycle++;
		if (cycle > cycleSpeed) cycle = 0;
		const auto delta = sin((float)cycle / (float)cycleSpeed * 2.f * M_PI) * 0.5f + 0.5f;
		setPosition(Util::lerp(static_cast<float>(delta), first, second));
	}
}

void PlayerPreviewWidget::setCosmetics(const PlayerCosmetics& cosmetics) {
	static sf::Sprite playerSprite(TextureHolder::getInstance().getDefault(), sf::IntRect({0, 0}, {128, 128}));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		playerSprite.setScale({2.f, 2.f});
	});
	sf::RenderTexture combinedImage({128, 128});
	playerSprite.setColor(cosmetics.colour);
	playerSprite.setTexture(TextureHolder::getInstance().get(*cosmetics.character));
	combinedImage.draw(playerSprite);
	playerSprite.setTexture(TextureHolder::getInstance().get(*cosmetics.cosmetic));
	combinedImage.draw(playerSprite);
	this->texture = sf::Texture();
	auto image = combinedImage.getTexture().copyToImage();
	image.flipVertically();
	if (!texture.loadFromImage(image)) {
		Logger::errorOnce("Failed to refresh Player Preview Widget Texture");
		return;
	}
	this->setTexture(&this->texture);
}