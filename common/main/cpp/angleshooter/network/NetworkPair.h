#pragma once

class NetworkPair {
	std::unordered_map<uint32_t, std::pair<sf::Packet, std::chrono::steady_clock::time_point>> sentPackets;
	SocketHolder& socketHolder;
	PortedIP pip;
	sf::Clock roundTripTimer;
	float rtt = 0;
	uint32_t nextSequence = 0;
	uint32_t acknowledgedSequence = 0;
	const std::chrono::milliseconds resendInterval{500};
	sf::Clock lastResponse;
	uint32_t playerId = -1;
	bool disconnecting = false;

	void sendPacketInternal(sf::Packet& packet);

public:
	explicit NetworkPair(SocketHolder& socketHolder, PortedIP pip);

	void update();
	void send(sf::Packet& packet);
	void acceptAcknowledgment(uint32_t sequence);

	[[nodiscard]] unsigned int getPlayerId() const;
	void setPlayerId(uint32_t id);

	void startRoundTripTimer();
	float stopRoundTripTimer();

	[[nodiscard]] float getTimeoutRemaining() const;
	void resetTimeout();

	[[nodiscard]] uint32_t getAcknowledgedSequence() const;
	bool setAcknowledgedSequence(uint32_t sequence);

	[[nodiscard]] bool shouldDisconnect() const;
	void setDisconnecting();

	[[nodiscard]] uint32_t getNextSequence();
	[[nodiscard]] PortedIP getPortedIP() const;
};