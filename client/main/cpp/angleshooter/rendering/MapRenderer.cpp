#include "PreCompiledClient.h"
#include "MapRenderer.h"

MapRenderer::MapRenderer(Map& map) {
	this->mapTexture = new sf::RenderTexture({static_cast<unsigned int>(map.getColumns() * 16), static_cast<unsigned int>(map.getRows() * 16)});
	for (uint16_t row = 0; row < map.getRows(); ++row) {
		for (uint16_t column = 0; column < map.getColumns(); ++column) {
			sf::Sprite tile(TextureHolder::getInstance().get(map.getTextures()[map.getTextureGrid()[row][column]]));
			tile.setPosition({static_cast<float>(column * 16), static_cast<float>(row * 16)});
			const auto tileSize = tile.getTexture().getSize();
			tile.setScale({16.f / static_cast<float>(tileSize.x), 16.f / static_cast<float>(tileSize.y)});
			const auto color = static_cast<uint8_t>(map.isSolid(row, column) ? 255 : 128);
			tile.setColor(sf::Color(color, color, color, 255));
			this->mapTexture->draw(tile);
		}
	}
	this->mapSprite = new sf::Sprite(this->mapTexture->getTexture());
	this->mapSprite->setScale({1.f, -1.f});
	this->mapSprite->setPosition({0.f, static_cast<float>(map.getRows() * 16)});
}

void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*this->mapSprite);
}