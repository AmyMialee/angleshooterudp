#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "NetworkPair.h"

NetworkPair::NetworkPair(SocketHolder& socketHolder, PortedIP pip) : socketHolder(socketHolder), pip(pip) {
	const auto packet = NetworkProtocol::PING->getPacket();
	this->send(packet);
	this->lastResponse.restart();
}

void NetworkPair::update() {
	const auto now = std::chrono::steady_clock::now();
	for (auto& pair : sentPackets | std::views::values) {
		if (auto& [packet, timestamp] = pair; now - timestamp > resendInterval) {
			sendPacketInternal(packet);
			timestamp = now;
		}
	}
	if (this->lastSendTimer.getElapsedTime().asSeconds() > AngleShooterCommon::TIMEOUT / 3) {
		const auto packet = NetworkProtocol::HEARTBEAT->getPacket();
		send(packet);
	}
}

void NetworkPair::send(const OutputBitStream& packet) {
	const auto type = packet.getBuffer()[0];
	if (const auto id = PacketIdentifier::fromId(type); id->isReliable()) {
		const auto body = packet.getBuffer() + 1;
		const auto bodySize = packet.getByteLength() - 1;
		OutputBitStream updated;
		updated << type;
		const auto sequence = this->getNextSequence();
		updated << sequence;
		updated.writeBits(body, bodySize * 8);
		sentPackets[sequence] = {updated, std::chrono::steady_clock::now()};
		sendPacketInternal(updated);
	} else {
		sendPacketInternal(packet);
	}
}

void NetworkPair::acceptAcknowledgment(uint32_t sequence) {
	this->sentPackets.erase(sequence);
}

void NetworkPair::setPlayerId(uint32_t id) {
	this->playerId = id;
}

uint32_t NetworkPair::getPlayerId() const {
	return this->playerId;
}

void NetworkPair::startRoundTripTimer() {
	this->roundTripTimer.restart();
}

float NetworkPair::stopRoundTripTimer() {
	this->rtt = this->roundTripTimer.getElapsedTime().asSeconds();
	return this->rtt;
}

float NetworkPair::getTimeoutRemaining() const {
	return AngleShooterCommon::TIMEOUT - this->lastResponse.getElapsedTime().asSeconds();
}

void NetworkPair::resetTimeout() {
	this->lastResponse.restart();
}

uint32_t NetworkPair::getAcknowledgedSequence() const {
	return this->acknowledgedSequence;
}

bool NetworkPair::setAcknowledgedSequence(uint32_t sequence) {
	if (this->acknowledgedSequence != sequence) return false;
	this->acknowledgedSequence++;
	return true;
}

uint32_t NetworkPair::getNextSequence() {
	return this->nextSequence++;
}

bool NetworkPair::shouldDisconnect() const {
	return this->disconnecting || this->lastResponse.getElapsedTime().asSeconds() > AngleShooterCommon::TIMEOUT;
}

void NetworkPair::setDisconnecting() {
	this->disconnecting = true;
}

PortedIP NetworkPair::getPortedIP() const {
	return pip;
}

void NetworkPair::sendPacketInternal(const OutputBitStream& packet) {
	this->lastSendTimer.restart();
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socketHolder.getSocket().send(packet.getBuffer(), packet.getByteLength(), pip.ip, pip.port);
	if (status != sf::Socket::Status::Done) Logger::error("Send Error: " + pip.toString());
	if (status == sf::Socket::Status::Disconnected) {
		Logger::error("Disconnected from " + pip.toString());
		this->disconnecting = true;
	}
}