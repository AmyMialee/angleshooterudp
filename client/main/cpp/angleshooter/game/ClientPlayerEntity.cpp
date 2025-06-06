#include "PreCompiledClient.h"
#include "ClientPlayerEntity.h"

ClientPlayerEntity::ClientPlayerEntity(uint16_t id, World* world) : PlayerEntity(id, world) {}

void ClientPlayerEntity::tick() {
	PlayerEntity::tick();
	if (firingInput.length() > 0.5f) {  // NOLINT(clang-diagnostic-undefined-func-template)
		firingInput /= firingInput.length();
		this->targetRotation = sf::radians(std::atan2(firingInput.y, firingInput.x));
	}
	auto rotationDifference = targetRotation - weaponRotation;
	if (rotationDifference.asRadians() > std::numbers::pi) {
		rotationDifference -= sf::radians(2 * static_cast<float>(std::numbers::pi));
	} else if (rotationDifference.asRadians() < -std::numbers::pi) {
		rotationDifference += sf::radians(2 * static_cast<float>(std::numbers::pi));
	}
	this->weaponSpin += rotationDifference * 0.1f;
	this->weaponSpin *= 0.9f;
	this->weaponRotation += this->weaponSpin;
	if (!isClientPlayer) return;
	input = sf::Vector2f(0, 0);
	if (InputManager::get().getUp()->isPressed()) input += sf::Vector2f(0, -1);
	if (InputManager::get().getDown()->isPressed()) input += sf::Vector2f(0, 1);
	if (InputManager::get().getLeft()->isPressed()) input += sf::Vector2f(-1, 0);
	if (InputManager::get().getRight()->isPressed()) input += sf::Vector2f(1, 0);
	firingInput = sf::Vector2f(0, 0);
	if (InputManager::get().getFireUp()->isPressed()) firingInput += sf::Vector2f(0, -1);
	if (InputManager::get().getFireDown()->isPressed()) firingInput += sf::Vector2f(0, 1);
	if (InputManager::get().getFireLeft()->isPressed()) firingInput += sf::Vector2f(-1, 0);
	if (InputManager::get().getFireRight()->isPressed()) firingInput += sf::Vector2f(1, 0);
	if (syncedInput != input || syncedFiring != firingInput) {
		auto packetIn = NetworkProtocol::C2S_PLAYER_INPUT->getPacket();
		packetIn << input.x << input.y << firingInput.x << firingInput.y;
		AngleShooterClient::get().send(packetIn);
		this->syncedInput = input;
		this->syncedFiring = firingInput;
		AudioManager::get().setListenerPosition(this->getPosition());
	}
	if (this->world->getAge() % 8 == 0 && this->getPosition() != this->syncedPosition) {
		auto packet = NetworkProtocol::C2S_PLAYER_POSITION_SYNC->getPacket();
		packet << this->getPosition().x;
		packet << this->getPosition().y;
		AngleShooterClient::get().send(packet);
		this->syncedPosition = this->getPosition();
	}
}

void ClientPlayerEntity::readFromPacket(InputBitStream& packet) {
	PlayerEntity::readFromPacket(packet);
	if (const auto it = GameManager::get().SCORES.find(this->getId()); it != GameManager::get().SCORES.end()) {
		it->second.score = this->score;
	} else {
		GameManager::get().SCORES.emplace(this->getId(), ScoreEntry{this->name, this->cosmetics, this->score, 0, 0});
	}
	GameManager::get().refreshScores();
}