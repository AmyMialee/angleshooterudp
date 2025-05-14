#pragma once

class AngleShooterClient final : public SocketHolder {
	std::map<uint8_t, std::function<void(InputBitStream& packet, NetworkPair* pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	sf::UdpSocket socket;

	void render();
	void tickNetwork();

	void handlePacket(InputBitStream& packet, NetworkPair* sender);
	void registerPackets();
	void registerPacket(PacketIdentifier* packetType, const std::function<void(InputBitStream& packet, NetworkPair* sender)>& handler);

protected:
	AngleShooterClient();
	~AngleShooterClient() override = default;

public:
	NetworkPair* server = nullptr;
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Default);
	double fps = 0;
	double tps = 0;
	double lps = 0;
	double tickDelta = 0;
	uint16_t playerId;
	bool debug = false;
	bool hitboxes = false;
	bool onMainMenu = true;

	void run();
	void send(OutputBitStream& packet);
	void connect(const PortedIP& server);
	void disconnect();

	sf::UdpSocket& getSocket() override;
	AngleShooterClient(const AngleShooterClient&) = delete;
	void operator=(const AngleShooterClient&) = delete;
	static AngleShooterClient& get() {
		static AngleShooterClient instance;
		return instance;
	}
};