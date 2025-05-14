#pragma once

class PlayerEntity : public Entity {
	bool isMarkedForRemoval() const override;

public:
	std::string name;
	sf::Vector2f input = {0, 0};
	sf::Vector2f firingInput = {0, 0};
	PlayerCosmetics cosmetics;
	uint16_t health = 8;
	uint16_t deathTime = 0;
	uint16_t immunityTime = 0;
	uint16_t bulletCharge = 0;
	uint16_t score = 0;

	explicit PlayerEntity(uint16_t id, World* world);
	inline static auto ID = Identifier("player");
	const Identifier& getEntityType() const override;
	void tick() override;
	virtual bool damage(uint16_t source, uint16_t amount);
	virtual void onDeath(uint16_t source);

	void writeToPacket(OutputBitStream& packet) const override;
	void readFromPacket(InputBitStream& packet) override;
};