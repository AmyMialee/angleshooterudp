﻿#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(uint16_t id, World* world) : Entity(id, world) {
	this->setDrag(0.35f);
	this->setScale({14, 14});
}

const Identifier& PlayerEntity::getEntityType() const {
	return ID;
}

void PlayerEntity::tick() {
	if (this->deathTime > 0) {
		this->deathTime--;
		return;
	}
	if (this->immunityTime > 0) {
		this->immunityTime--;
		this->bulletCharge = 0;
	} else {
		this->bulletCharge += AngleShooterCommon::BULLET_CHARGE;
		this->bulletCharge = std::min(this->bulletCharge, static_cast<uint16_t>(AngleShooterCommon::MAX_BULLETS));
	}
	if (input.length() > 0) {  // NOLINT(clang-diagnostic-undefined-func-template)
		input /= input.length();
		constexpr auto movementSpeed = 1.6f;
		input *= movementSpeed;
		this->addVelocity(input);
		const auto currentRotation = this->getRotation();
		const auto targetRotation = sf::radians(std::atan2(input.y, input.x)) + sf::degrees(-90);
		auto rotationDifference = targetRotation - currentRotation;
		if (rotationDifference.asRadians() > std::numbers::pi) {
			rotationDifference -= sf::radians(2 * static_cast<float>(std::numbers::pi));
		} else if (rotationDifference.asRadians() < -std::numbers::pi) {
			rotationDifference += sf::radians(2 * static_cast<float>(std::numbers::pi));
		}
		const auto newRotation = currentRotation + rotationDifference * 0.75f;
		this->setRotation(newRotation);
	}
	if (this->firingInput.length() > 0) {
		//TODO: Rotate Weapon
	}
	Entity::tick();
}

bool PlayerEntity::damage(uint16_t source, uint16_t amount) {
	if (this->health <= 0 || this->immunityTime > 0) return false;
	this->health -= amount;
	if (this->health <= 0) {
		this->health = 0;
		this->onDeath(source);
	}
	return true;
}

void PlayerEntity::onDeath(uint16_t source) {
	this->deathTime = 60;
	this->immunityTime = 120;
}

bool PlayerEntity::isMarkedForRemoval() const {
	return false;
}

void PlayerEntity::writeToPacket(OutputBitStream& packet) const {
	Entity::writeToPacket(packet);
	packet << this->name;
	packet << this->cosmetics;
	packet << this->health;
	packet << this->input.x;
	packet << this->input.y;
	packet << this->getVelocity().x;
	packet << this->getVelocity().y;
	packet << this->score;
}

void PlayerEntity::readFromPacket(InputBitStream& packet) {
	Entity::readFromPacket(packet);
	packet >> this->name;
	packet >> this->cosmetics;
	packet >> this->health;
	packet >> this->input.x;
	packet >> this->input.y;
	float vx, vy;
	packet >> vx;
	packet >> vy;
	this->setVelocity({vx, vy});
	packet >> this->score;
}