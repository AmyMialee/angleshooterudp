#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "World.h"

World::World() : map(Map(Identifier("empty"), 1, 1)) {}

void World::init() {
	this->gameObjects.clear();
}

void World::tick() {
	this->age++;
	for (auto main = this->gameObjects.begin(); main != this->gameObjects.end(); ++main) {
		if (!main->second) continue;
		main->second->tick();
	}
}

std::shared_ptr<Entity> World::spawnEntity(std::shared_ptr<Entity> entity) {
	this->gameObjects.emplace(entity->getId(), std::move(entity));
	return entity;
}

std::vector<std::shared_ptr<Entity>> World::getEntities() {
	std::vector<std::shared_ptr<Entity>> values;
	values.reserve(this->gameObjects.size());
	for (auto main = this->gameObjects.begin(); main != this->gameObjects.end(); ++main) values.push_back(main->second);
	return values;
}

Map* World::getMap() {
	return &this->map;
}

uint16_t World::getNextId() {
	return ++this->nextId;
}

int World::getAge() const {
	return this->age;
}

void World::loadMap(const Identifier& id) {
	this->map = MapLoader::loadMap(id);
}
