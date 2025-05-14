#pragma once

class MapRenderer : public sf::Drawable {
	sf::RenderTexture* mapTexture;
	sf::Sprite* mapSprite;
	
public:
	explicit MapRenderer(Map& map);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};