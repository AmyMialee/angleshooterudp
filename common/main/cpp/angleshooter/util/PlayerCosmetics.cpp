#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerCosmetics.h"

sf::Packet& operator<<(sf::Packet& packet, const PlayerCosmetics& cosmetics) {
	packet << cosmetics.colour.r << cosmetics.colour.g << cosmetics.colour.b;
	packet << PlayerCosmetics::getCharacterIndex(*cosmetics.character);
	packet << PlayerCosmetics::getCosmeticIndex(*cosmetics.cosmetic);
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerCosmetics& id) {
	uint8_t r, g, b;
	packet >> r >> g >> b;
	uint8_t characterIndex, cosmeticIndex;
	packet >> characterIndex >> cosmeticIndex;
	id.character = PlayerCosmetics::getCharacter(characterIndex);
	id.cosmetic = PlayerCosmetics::getCosmetic(cosmeticIndex);
	return packet;
}