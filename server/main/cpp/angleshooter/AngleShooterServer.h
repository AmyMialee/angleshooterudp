#pragma once

class AngleShooterServer final : public SocketHolder {
	std::map<uint8_t, std::function<void(InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	sf::UdpSocket listenerSocket;
	bool running = true;

	void tickNetwork();
	void handlePacket(InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender);
	void registerPacket(PacketIdentifier* packetType, const std::function<void(InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender)>& handler);

protected:
	AngleShooterServer();
	~AngleShooterServer() override = default;

public:
	std::map<PortedIP, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>> clients;
	double tps;
	double lps;

	void run();
	void sendToAll(const OutputBitStream& packet, const std::function<bool(const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>&)>& predicate = [](const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>&) { return true; });
	void send(OutputBitStream packet, const std::unique_ptr<NetworkPair>& pair);

	sf::UdpSocket& getSocket() override;
	AngleShooterServer(const AngleShooterServer&) = delete;
	void operator=(const AngleShooterServer&) = delete;
	static AngleShooterServer& get() {
		static AngleShooterServer instance;
		return instance;
	}
};