#include "PreCompiledClient.h"
#include "ClientPlayerEntity.h"

ClientPlayerEntity::ClientPlayerEntity(uint16_t id, World* world) : PlayerEntity(id, world) {}

void ClientPlayerEntity::tick() {
	PlayerEntity::tick();
	if (!isClientPlayer || StateManager::get().getStateId() != GameState::GAME_ID) return;
	input = sf::Vector2f(0, 0);
	if (InputManager::get().getUp()->isPressed()) input += sf::Vector2f(0, -1);
	if (InputManager::get().getDown()->isPressed()) input += sf::Vector2f(0, 1);
	if (InputManager::get().getLeft()->isPressed()) input += sf::Vector2f(-1, 0);
	if (InputManager::get().getRight()->isPressed()) input += sf::Vector2f(1, 0);
	isFiring = InputManager::get().getFire()->isPressed();
	if (syncedInput != input || syncedFiring != isFiring) {
		auto packetIn = NetworkProtocol::C2S_PLAYER_INPUT->getPacket();
		packetIn << input.x << input.y << isFiring;
		AngleShooterClient::get().send(packetIn);
		this->syncedInput = input;
		this->syncedFiring = isFiring;
		AudioManager::get().setListenerPosition(this->getPosition());
		// AudioManager::get().setListenerRotation(input);
	}
	if (this->world->getAge() % 8 == 0 && this->getPosition() != this->syncedPosition) {
		auto packet = NetworkProtocol::C2S_PLAYER_POSITION_SYNC->getPacket();
		packet << this->getPosition().x;
		packet << this->getPosition().y;
		AngleShooterClient::get().send(packet);
		this->syncedPosition = this->getPosition();
	}
}

void ClientPlayerEntity::readFromPacket(sf::Packet& packet) {
	PlayerEntity::readFromPacket(packet);
	if (const auto it = GameState::SCORES.find(this->getId()); it != GameState::SCORES.end()) {
		it->second.score = this->score;
	} else {
		GameState::SCORES.emplace(this->getId(), ScoreEntry{this->name, this->colour, this->score, 0, 0});
	}
	GameState::refreshScores();
}