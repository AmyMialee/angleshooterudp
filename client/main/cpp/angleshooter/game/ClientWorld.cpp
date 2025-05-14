#include <PreCompiledClient.h>
#include "ClientWorld.h"

void ClientWorld::loadMap() {
	World::loadMap(this->queuedWorld);
	mapRenderer = new MapRenderer(*getMap());
}

ClientWorld::ClientWorld() : mapRenderer() {}

std::shared_ptr<ClientPlayerEntity> ClientWorld::spawnPlayer(InputBitStream& packet) {
	uint16_t id;
	packet >> id;
	const auto player = std::make_shared<ClientPlayerEntity>(id, this);
	player->readFromPacket(packet);
	this->spawnEntity(player);
	return player;
}

std::shared_ptr<BulletEntity> ClientWorld::spawnBullet(InputBitStream& packet) {
	uint16_t id;
	packet >> id;
	const auto bullet = std::make_shared<BulletEntity>(id, this);
	bullet->readFromPacket(packet);
	this->spawnEntity(bullet);
	return bullet;
}

void ClientWorld::tick() {
	World::tick();
	if (this->queuedWorld != Identifier::empty) {
		this->loadMap();
		this->queuedWorld = Identifier::empty;
	}
}

void ClientWorld::playMusic(const Identifier& id, float volume, float pitch) {
	AudioManager::get().playMusic(id, volume, pitch);
}

void ClientWorld::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	AudioManager::get().playSound(id, volume, pitch, position, attenuation);
}

void ClientWorld::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	AudioManager::get().playSound3d(id, volume, pitch, position, attenuation);
}

void ClientWorld::loadMap(const Identifier& id) {
	this->queuedWorld = id;
}