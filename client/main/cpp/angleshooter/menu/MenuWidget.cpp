#include "PreCompiledClient.h"
#include "MenuWidget.h"

MenuWidget::MenuWidget(sf::Vector2f position, sf::Vector2f size, const Identifier& texture) {
	this->setPosition(position);
	this->setSize(size);
	Util::centre(*this);
	if (texture != Identifier::empty) this->setTexture(TextureHolder::getInstance().getPointer(texture));
}

MenuWidget::MenuWidget(sf::Vector2f position, float size, const Identifier& texture) {
	this->setPosition(position);
	if (texture != Identifier::empty) {
		const auto texturePointer = TextureHolder::getInstance().getPointer(texture);
		this->setSize({size, size * texturePointer->getSize().y / texturePointer->getSize().x});
		Util::centre(*this);
		this->setTexture(texturePointer);
	}
}