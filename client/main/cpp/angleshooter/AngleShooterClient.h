#pragma once

class AngleShooterClient final : public SocketHolder {
	std::map<uint8_t, std::function<void(sf::Packet& packet, NetworkPair* pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	sf::UdpSocket socket;
	std::vector<PortedIP> servers;
	MenuManager mainMenuManager;
	MenuPage* serverListPage;
	bool onMainMenu = true;

	void render();
	void populateMainMenu();
	void populateServerPage();
	void runReceiver();

	void handlePacket(sf::Packet& packet, NetworkPair* sender);
	void registerPackets();
	void registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler);

protected:
	AngleShooterClient();
	~AngleShooterClient() override = default;

public:
	NetworkPair* server = nullptr;
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Default);
	sf::RenderTexture renderTexture = sf::RenderTexture({980, 540});
	double fps = 0;
	double tps = 0;
	double lps = 0;
	double tickDelta = 0;
	uint16_t playerId;
	bool debug = false;
	bool hitboxes = false;

	void run();
	void send(sf::Packet& packet);
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