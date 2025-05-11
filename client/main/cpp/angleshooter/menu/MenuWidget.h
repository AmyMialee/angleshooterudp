#pragma once

struct MenuWidget : sf::RectangleShape {
	virtual void tick() {}

	explicit MenuWidget(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, const Identifier& texture = Identifier::empty) {
		this->setPosition(position);
		this->setSize(size);
		Util::centre(*this);
		if (texture != Identifier::empty) this->setTexture(TextureHolder::getInstance().getPointer(texture));
	}

	explicit MenuWidget(sf::Vector2f position = {0, 0}, float size = 30, const Identifier& texture = Identifier::empty) {
		this->setPosition(position);
		if (texture != Identifier::empty) {
			const auto texturePointer = TextureHolder::getInstance().getPointer(texture);
			this->setSize({size, size * texturePointer->getSize().y / texturePointer->getSize().x});
			Util::centre(*this);
			this->setTexture(texturePointer);
		}
	}
};