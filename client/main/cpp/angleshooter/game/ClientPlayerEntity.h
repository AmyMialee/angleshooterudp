#pragma once

class ClientPlayerEntity final : public PlayerEntity {
	sf::Vector2f syncedFiring = sf::Vector2f(0, 0);
	sf::Vector2f syncedInput = sf::Vector2f(0, 0);
	sf::Vector2f syncedPosition = sf::Vector2f(0, 0);
public:
	sf::Angle weaponRotation = sf::degrees(0);
	sf::Angle weaponSpin = sf::degrees(0);
	sf::Angle targetRotation = sf::degrees(0);

	bool isClientPlayer = false;
	explicit ClientPlayerEntity(uint16_t id, World* world);
	void tick() override;
	void readFromPacket(InputBitStream& packet) override;
};