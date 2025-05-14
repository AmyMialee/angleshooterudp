#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

int main() {
	try {
		AngleShooterServer::get().run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterServer::AngleShooterServer() {
    NetworkProtocol::initialize();
    listenerSocket.setBlocking(false);
    if (listenerSocket.bind(AngleShooterCommon::PORT) != sf::Socket::Status::Done) throw std::runtime_error("Failed to bind to port, is the server already running?");
	Logger::info("Server started on port " + std::to_string(listenerSocket.getLocalPort()));
	registerPacket(NetworkProtocol::PING, [this](sf::Packet&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        Logger::debug("Ping! from " + sender.first->getPortedIP().toString());
        const auto pong = NetworkProtocol::PONG->getPacket();
        send(pong, sender.first);
	});
	registerPacket(NetworkProtocol::PONG, [this](sf::Packet&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		const auto rtt = sender.first->stopRoundTripTimer();
		Logger::debug("Pong! from " + sender.first->getPortedIP().toString() + " in " + Util::toRoundedString(rtt * 1000, 0) + "ms");
	});
	registerPacket(NetworkProtocol::ACK, [this](sf::Packet& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		uint32_t sequence;
		packet >> sequence;
		sender.first->acceptAcknowledgment(sequence);
	});
	registerPacket(NetworkProtocol::CHAT_MESSAGE, [this](sf::Packet& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		std::string message;
		packet >> message;
		Logger::info("Received Chat Message Packet from " + sender.first->getPortedIP().toString() + ": " + message);
		auto broadcastPacket = NetworkProtocol::CHAT_MESSAGE->getPacket();
		broadcastPacket << ("<" + sender.first->getPortedIP().toString() + "> " + message);
		sendToAll(broadcastPacket);
	});
	registerPacket(NetworkProtocol::C2S_JOIN, [this](sf::Packet& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        std::string name;
        PlayerCosmetics cosmetics;
        packet >> name;
        packet >> cosmetics;
        auto endName = name;
        auto count = 0;
        for (const auto& [pair, details] : clients | std::views::values) if (details.name == endName) endName = name + " " + std::to_string(++count);
        Logger::debug("Received Join Packet from " + endName + " (" + sender.first->getPortedIP().toString() + ")");
        sender.second.name = endName;
        sender.second.cosmetics = cosmetics;
        auto mapPacket = NetworkProtocol::S2C_INITIAL_SETUP->getPacket();
        mapPacket << ServerWorld::get().getMap()->getId();
        sender.second.player = ServerWorld::get().spawnPlayer(sender);
        mapPacket << sender.second.player->getId();
        send(mapPacket, sender.first);
        for (const auto& [pair, details] : clients | std::views::values) {
            if (!details.player) return;
            if (details.player->getId() == sender.second.player->getId()) continue;
            auto syncPlayerPacket = NetworkProtocol::S2C_SPAWN_PLAYER->getPacket();
            details.player->writeToPacket(syncPlayerPacket);
            syncPlayerPacket << false;
            send(syncPlayerPacket, pair);
        }
    });
    registerPacket(NetworkProtocol::C2S_SEND_MESSAGE, [this](sf::Packet& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        std::string message;
        packet >> message;
        Logger::debug("Received Send Message Packet from " + sender.second.name + " (" + sender.first->getPortedIP().toString() + "): " + message);
        auto broadcastPacket = NetworkProtocol::S2C_BROADCAST_MESSAGE->getPacket();
        broadcastPacket << "<" << sender.second.name << ">: " << message;
        sendToAll(broadcastPacket);
    });
    registerPacket(NetworkProtocol::C2S_QUIT, [this](sf::Packet&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        Logger::debug("Received Quit Packet from " + sender.second.name + "(" + sender.first->getPortedIP().toString() + ")");
        sender.first->setDisconnecting();
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_INPUT, [this](sf::Packet& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        float x, y;
        bool isFiring;
        packet >> x;
        packet >> y;
        packet >> isFiring;
        sender.second.player->input = {x, y};
        sender.second.player->isFiring = isFiring;
        auto syncPlayerPacket = NetworkProtocol::S2C_PLAYER_INPUT->getPacket();
        syncPlayerPacket << sender.second.player->getId();
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        syncPlayerPacket << isFiring;
        sendToAll(syncPlayerPacket, [&sender](const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& client) {
            if (!client.second.player) return true;
            if (client.second.player->getId() == sender.second.player->getId()) return false;
            return true;
        });
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_POSITION_SYNC, [this](sf::Packet& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        float x, y;
        packet >> x;
        packet >> y;
        sender.second.player->setPosition({x, y});
        auto syncPlayerPacket = NetworkProtocol::S2C_PLAYER_POSITION_SYNC->getPacket();
        syncPlayerPacket << sender.second.player->getId();
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        sendToAll(syncPlayerPacket, [&sender](const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& client) {
            if (!client.second.player) return true;
            if (client.second.player->getId() == sender.second.player->getId()) return false;
            return true;
        });
    });
    registerPacket(NetworkProtocol::C2S_UPDATE_NAME, [this](sf::Packet& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        std::string name;
        packet >> name;
        auto endName = name;
        auto count = 0;
        for (const auto& [pair, details] : clients | std::views::values) if (details.name == endName) endName = name + " " + std::to_string(++count);
        Logger::debug("Received Change Name Packet " + endName + " (" + sender.first->getPortedIP().toString() + "), previously " + sender.second.name);
        sender.second.name = endName;
        auto syncNamePacket = NetworkProtocol::S2C_UPDATE_NAME->getPacket();
        syncNamePacket << sender.second.player->getId();
        syncNamePacket << name;
        sendToAll(syncNamePacket);
    });
    registerPacket(NetworkProtocol::C2S_UPDATE_COSMETICS, [this](sf::Packet& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        packet >> sender.second.cosmetics;
        Logger::debug("Received Change Cosmetics Packet (" + sender.first->getPortedIP().toString() + ")");
        auto syncColourPacket = NetworkProtocol::S2C_UPDATE_COSMETICS->getPacket();
        syncColourPacket << sender.second.player->getId();
        syncColourPacket << sender.second.cosmetics;
        sendToAll(syncColourPacket);
    });
}

void AngleShooterServer::handlePacket(sf::Packet& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
	sender.first->resetTimeout();
    uint8_t packetType;
	packet >> packetType;
	if (PacketIdentifier::fromId(packetType)->isReliable()) {
		uint32_t sequence;
		packet >> sequence;
		if (sequence < sender.first->getAcknowledgedSequence()) {
			auto ack = NetworkProtocol::ACK->getPacket();
			ack << sequence;
			send(ack, sender.first);
			Logger::debug("Received redundant sequence: " + std::to_string(sequence) + " from " + sender.first->getPortedIP().toString());
			return;
		}
		if (sender.first->setAcknowledgedSequence(sequence)) {
			auto ack = NetworkProtocol::ACK->getPacket();
			ack << sequence;
			send(ack, sender.first);
		} else {
			Logger::debug("Received premature sequence: " + std::to_string(sequence) + " from " + sender.first->getPortedIP().toString());
			return;
		}
	}
    if (packetHandlers.contains(packetType)) {
        packetHandlers[packetType](packet, sender);
        if (packet.getReadPosition() != packet.getDataSize()) {
            Logger::error("Packet " + packetIds[packetType]->toString() + " has unused data");
            packet.clear();
        }
        return;
    }
	Logger::error("Received unknown packet id: " + std::to_string(packetType) + " from " + sender.first->getPortedIP().toString());
}

void AngleShooterServer::registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender)>& handler) {
	this->packetHandlers.emplace(packetType->getId(), handler);
	this->packetIds.emplace(packetType->getId(), packetType);
	Logger::debug("Registered packet: " + packetType->toString() + " (" + std::to_string(packetType->getId()) + ")");
}

void AngleShooterServer::run() {
	Logger::info("Starting AngleShooter Server");
	ServerWorld::get().init();
	ServerWorld::get().loadMap(Identifier("testmaplarge"));
    std::thread receiverThread(&AngleShooterServer::runReceiver, this);
    sf::Clock deltaClock;
    auto tickTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
	auto loops = 0;
    Logger::info("Starting Server Game Loop");
    while (this->running) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        secondTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK) + " ticks (" + Util::toRoundedString(tickTime) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            tickTime -= AngleShooterCommon::TIME_PER_TICK;
            ServerWorld::get().tick();
            ++ticks;
        }
		++loops;
        if (secondTime >= .1f) {
            tps = ticks / secondTime;
            lps = loops / secondTime;
            tps = tps * .8 + .2 * (ticks / secondTime);
            lps = lps * .8 + .2 * (loops / secondTime);
            ticks = 0;
            loops = 0;
            secondTime = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
    }
}

void AngleShooterServer::runReceiver() {
    Logger::info("Starting Server Network Handler");
	std::set<PortedIP> pendingDisconnects;
	while (this->running) {
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; listenerSocket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			if (!sender.has_value()) continue;
			if (const auto bytes = static_cast<const uint8_t*>(packet.getData()); PacketIdentifier::fromId(bytes[0]) == NetworkProtocol::SERVER_SCAN) {
				auto scanPacket = NetworkProtocol::SERVER_SCAN->getPacket();
				auto status = sf::Socket::Status::Partial;
				while (status == sf::Socket::Status::Partial) status = this->getSocket().send(scanPacket, sender.value(), port);
				Logger::debug("Server scanned by " + sender.value().toString());
				continue;
			}
			auto pip = PortedIP{.ip= sender.value(), .port= port};
			if (!clients.contains(pip)) {
				clients.emplace(pip, std::pair(std::make_unique<NetworkPair>(*this, pip), PlayerDetails()));
				Logger::info("Accepted connection from: " + pip.toString());
			}
			handlePacket(packet, clients[pip]);
			continue;
		}
		for (const auto& [first, second] : clients | std::views::values) {
			first->update();
			if (first->shouldDisconnect()) pendingDisconnects.insert(first->getPortedIP());
		}
		auto iterator = clients.begin();
		while (iterator != clients.end()) {
			if (pendingDisconnects.contains(iterator->second.first->getPortedIP())) {
                Logger::info("Client disconnected: " + iterator->second.first->getPortedIP().toString());
				iterator = clients.erase(iterator);
			} else ++iterator;
		}
		pendingDisconnects.clear();
		sleep(sf::milliseconds(6));
    }
}

void AngleShooterServer::sendToAll(const sf::Packet& packet, const std::function<bool(const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>&)>& predicate) {
    for (const auto& pair : clients | std::views::values) {
        if (!predicate(pair)) continue;
        send(packet, pair.first);
    }
}

void AngleShooterServer::send(sf::Packet packet, const std::unique_ptr<NetworkPair>& pair) {
	pair->send(packet);
}

sf::UdpSocket& AngleShooterServer::getSocket() {
	return this->listenerSocket;
}