#include "PreCompiledClient.h"
#include "MapRenderer.h"

MapRenderer::MapRenderer(Map& map) {
	this->mapTexture = new sf::RenderTexture({static_cast<unsigned int>(map.getColumns() * 16), static_cast<unsigned int>(map.getRows() * 16)});
	const auto floor = TextureHolder::getInstance().getPointer(Identifier("mapassets/floor.png"));
	sf::Sprite floorTile(*floor);
	floorTile.setScale({16.f / floorTile.getTexture().getSize().x, 16.f / floorTile.getTexture().getSize().y});
	for (uint16_t row = 0; row < map.getRows(); ++row) {
		for (uint16_t column = 0; column < map.getColumns(); ++column) {
			{
				floorTile.setPosition({column * 16.f, row * 16.f});
				const auto color = static_cast<uint8_t>(map.isSolid(row, column) ? 255 : 128);
				floorTile.setColor(sf::Color(color, color, color, 255));
				this->mapTexture->draw(floorTile);
			}
			const auto texture = TextureHolder::getInstance().getPointer(map.getTextures()[map.getTextureGrid()[row][column]]);
			if (texture == nullptr) continue;
			if (texture->getSize().x != texture->getSize().y) {
				sf::Sprite tileMap(*texture);
				tileMap.setPosition({column * 16.f + 16.f, row * 16.f + 16.f});
				tileMap.rotate(sf::degrees(180));
				const auto tileSize = tileMap.getTexture().getSize();
				auto sides = this->getTileSides(map, row, column);
				auto uv = tileMaskToUV.find(sides);
				if (uv == tileMaskToUV.end()) {
					std::stringstream ss;
					ss << std::bitset<8>(sides);
					Logger::errorOnce("MapRenderer: Tile mask not found for tile with mask " + ss.str());
					continue;
				}
				tileMap.setTextureRect({{static_cast<int>(tileSize.x / 12) * uv->second.first, static_cast<int>(tileSize.y / 4) * uv->second.second}, {static_cast<int>(tileSize.x / 12), static_cast<int>(tileSize.y / 4)}});
				tileMap.setScale(sf::Vector2f{16.f, 16.f}.componentWiseDiv(sf::Vector2f{static_cast<float>(tileSize.x / 12), static_cast<float>(tileSize.y / 4)}));
				// const auto color = static_cast<uint8_t>(map.isSolid(row, column) ? 255 : 128);
				// tileMap.setColor(sf::Color(color, color, color, 255));
				this->mapTexture->draw(tileMap);
			} else {
				sf::Sprite tile(*texture);
				tile.setPosition({column * 16.f, row * 16.f});
				const auto tileSize = tile.getTexture().getSize();
				tile.setScale({16.f / tileSize.x, 16.f / tileSize.y});
				// const auto color = static_cast<uint8_t>(map.isSolid(row, column) ? 255 : 128);
				// tile.setColor(sf::Color(color, color, color, 255));
				this->mapTexture->draw(tile);
			}
		}
	}
	this->mapSprite = new sf::Sprite(this->mapTexture->getTexture());
	this->mapSprite->setScale({1.f, -1.f});
	this->mapSprite->setPosition({0.f, static_cast<float>(map.getRows() * 16)});
}

void MapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*this->mapSprite);
}

uint8_t MapRenderer::getTileSides(Map& map, uint16_t row, uint16_t column) const {
	const auto& textureGrid = map.getTextureGrid();
	const auto& textures = map.getTextures();
	const auto currentTexture = textures[textureGrid[row][column]];
	const auto currentWall = map.isSolid(row, column);
	uint8_t mask = 0;
	for (auto i = 0; i < 8; ++i) {
		const int dCol[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
		const int dRow[8] = {-1, -1, -1,  0, 0, 1, 1, 1};
		const auto nRow = row + dRow[i];
		const auto nCol = column + dCol[i];
		if (nRow < 0 || nRow >= map.getRows() || nCol < 0 || nCol >= map.getColumns()) continue;
		if (auto neighborTexture = textures[textureGrid[nRow][nCol]]; neighborTexture == currentTexture && map.isSolid(nRow, nCol) == currentWall) {
			const uint8_t bits[8] = {1, 2, 4, 8, 16, 32, 64, 128};
			mask |= bits[i];
		}
	}
	return mask;
}