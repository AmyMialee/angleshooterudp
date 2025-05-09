﻿#pragma once

class World {
	Map map;
	uint16_t nextId = 0;
	int age = 0;

public:
	std::unordered_map<uint16_t, std::shared_ptr<Entity>> gameObjects;
	World();
	virtual ~World() = default;
	virtual void tick();
	void init();
	std::shared_ptr<Entity> spawnEntity(std::shared_ptr<Entity> entity);
	[[nodiscard]] std::vector<std::shared_ptr<Entity>> getEntities();
	[[nodiscard]] Map* getMap();
	[[nodiscard]] uint16_t getNextId();
	[[nodiscard]] int getAge() const;

	virtual void playMusic(const Identifier& id, float volume = 1.f, float pitch = 1.f) = 0;
	virtual void playSound(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector2f position = sf::Vector2f(0.f, 0.f), float attenuation = .1f) = 0;
	virtual void playSound3d(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector3f position = sf::Vector3f(0.f, 0.f, 0.f), float attenuation = .1f) = 0;
	virtual void loadMap(const Identifier& id);
};