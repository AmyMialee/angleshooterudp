#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

#include <utility>

#include "../robocat/RoboCatServerPCH.hpp"

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
	registerPacket(NetworkProtocol::PING, [this](InputBitStream&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        Logger::debug("Ping! from " + sender.first->getPortedIP().toString());
        const auto pong = NetworkProtocol::PONG->getPacket();
        send(pong, sender.first);
	});
	registerPacket(NetworkProtocol::PONG, [this](InputBitStream&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		const auto rtt = sender.first->stopRoundTripTimer();
		Logger::debug("Pong! from " + sender.first->getPortedIP().toString() + " in " + Util::toRoundedString(rtt * 1000, 0) + "ms");
	});
	registerPacket(NetworkProtocol::ACK, [this](InputBitStream& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		uint32_t sequence;
		packet >> sequence;
		sender.first->acceptAcknowledgment(sequence);
	});
	registerPacket(NetworkProtocol::CHAT_MESSAGE, [this](InputBitStream& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
		std::string message;
		packet >> message;
		Logger::info("Received Chat Message Packet from " + sender.first->getPortedIP().toString() + ": " + message);
		auto broadcastPacket = NetworkProtocol::CHAT_MESSAGE->getPacket();
		broadcastPacket << ("<" + sender.first->getPortedIP().toString() + "> " + message);
		sendToAll(broadcastPacket);
	});
	registerPacket(NetworkProtocol::C2S_JOIN, [this](InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
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
            auto newPlayerPacket = NetworkProtocol::S2C_SPAWN_PLAYER->getPacket();
            sender.second.player->writeToPacket(newPlayerPacket);
            newPlayerPacket << false;
            send(newPlayerPacket, pair);
        	auto existingPlayerPacket = NetworkProtocol::S2C_SPAWN_PLAYER->getPacket();
			details.player->writeToPacket(existingPlayerPacket);
			existingPlayerPacket << false;
			send(existingPlayerPacket, sender.first);
        }
    });
    registerPacket(NetworkProtocol::C2S_SEND_MESSAGE, [this](InputBitStream& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        std::string message;
        packet >> message;
        Logger::debug("Received Send Message Packet from " + sender.second.name + " (" + sender.first->getPortedIP().toString() + "): " + message);
        auto broadcastPacket = NetworkProtocol::S2C_BROADCAST_MESSAGE->getPacket();
        broadcastPacket << "<" << sender.second.name << ">: " << message;
        sendToAll(broadcastPacket);
    });
    registerPacket(NetworkProtocol::C2S_QUIT, [this](InputBitStream&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        Logger::debug("Received Quit Packet from " + sender.second.name + "(" + sender.first->getPortedIP().toString() + ")");
        sender.first->setDisconnecting();
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_INPUT, [this](InputBitStream& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        float x, y;
        float firingX, firingY;
        packet >> x;
        packet >> y;
        packet >> firingX;
    	packet >> firingY;
        sender.second.player->input = {x, y};
        sender.second.player->firingInput = {firingX, firingY};
        auto syncPlayerPacket = NetworkProtocol::S2C_PLAYER_INPUT->getPacket();
        syncPlayerPacket << sender.second.player->getId();
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        syncPlayerPacket << firingX;
    	syncPlayerPacket << firingY;
        sendToAll(syncPlayerPacket, [&sender](const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& client) {
            if (!client.second.player) return true;
            if (client.second.player->getId() == sender.second.player->getId()) return false;
            return true;
        });
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_POSITION_SYNC, [this](InputBitStream& packet, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
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
    registerPacket(NetworkProtocol::C2S_UPDATE_NAME, [this](InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
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
    registerPacket(NetworkProtocol::C2S_UPDATE_COSMETICS, [this](InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
        packet >> sender.second.cosmetics;
        Logger::debug("Received Change Cosmetics Packet (" + sender.first->getPortedIP().toString() + ")");
        auto syncColourPacket = NetworkProtocol::S2C_UPDATE_COSMETICS->getPacket();
        syncColourPacket << sender.second.player->getId();
        syncColourPacket << sender.second.cosmetics;
        sendToAll(syncColourPacket);
    });
	registerPacket(NetworkProtocol::HEARTBEAT, [this](InputBitStream&, const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>&) {});
}

void AngleShooterServer::handlePacket(InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender) {
	sender.first->resetTimeout();
    uint8_t packetType;
	packet.read(packetType);
	if (PacketIdentifier::fromId(packetType)->isReliable()) {
		uint32_t sequence;
		packet.read(sequence);
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
        if (packet.getRemainingBitCount() > 0) Logger::error("Packet " + packetIds[packetType]->toString() + " has unused data");
        return;
    }
	Logger::error("Received unknown packet id: " + std::to_string(packetType) + " from " + sender.first->getPortedIP().toString());
}

void AngleShooterServer::registerPacket(PacketIdentifier* packetType, const std::function<void(InputBitStream& packet, std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>& sender)>& handler) {
	this->packetHandlers.emplace(packetType->getId(), handler);
	this->packetIds.emplace(packetType->getId(), packetType);
}

void AngleShooterServer::run() {
	Logger::info("Starting AngleShooter Server");
	ServerWorld::get().init();
	ServerWorld::get().loadMap(Identifier("e1m1"));
    sf::Clock deltaClock;
    auto tickTime = 0.;
	auto networkTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
	auto loops = 0;
    Logger::info("Starting Server Game Loop");
    while (this->running) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
    	networkTime += deltaTime;
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
    	if (networkTime >= AngleShooterCommon::TIME_PER_TICK / 2) {
			tickNetwork();
			networkTime -= AngleShooterCommon::TIME_PER_TICK / 2;
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

void AngleShooterServer::tickNetwork() {
	uint8_t packetStorage[1500];
	size_t capturedSize = 0;
	std::optional<sf::IpAddress> sender;
	unsigned short port;
	InputBitStream packet(packetStorage, static_cast<uint32_t>(capturedSize) * 8);
	std::set<PortedIP> pendingDisconnects;
	while (this->running) {
		capturedSize = 0;
		if (listenerSocket.receive(packetStorage, sizeof(packetStorage), capturedSize, sender, port) == sf::Socket::Status::Done) {
			if (!sender.has_value() || capturedSize == 0) continue;
			auto pip = PortedIP{.ip= sender.value(), .port= port};
			if (std::cmp_equal(capturedSize, -WSAECONNRESET)) {
				pendingDisconnects.insert(pip);
			} else if (capturedSize > 0) {
				packet.reset(static_cast<uint32_t>(capturedSize));
			}
			if (PacketIdentifier::fromId(packetStorage[0]) == NetworkProtocol::SERVER_SCAN) {
				auto scanPacket = NetworkProtocol::SERVER_SCAN->getPacket();
				auto status = sf::Socket::Status::Partial;
				while (status == sf::Socket::Status::Partial) status = this->getSocket().send(scanPacket.getBuffer(), scanPacket.getByteLength(), sender.value(), port);
				Logger::debug("Server scanned by " + sender.value().toString());
				continue;
			}
			if (!clients.contains(pip)) {
				clients.emplace(pip, std::pair(std::make_unique<NetworkPair>(*this, pip), PlayerDetails()));
				Logger::info("Accepted connection from: " + pip.toString());
			}
			handlePacket(packet, clients[pip]);
			continue;
		}
		break;
	}
	for (const auto& [first, second] : clients | std::views::values) {
		first->update();
		if (first->shouldDisconnect()) pendingDisconnects.insert(first->getPortedIP());
	}
	auto iterator = clients.begin();
	while (iterator != clients.end()) {
		if (pendingDisconnects.contains(iterator->second.first->getPortedIP())) {
               Logger::info("Client disconnected: " + iterator->second.first->getPortedIP().toString());
			if (iterator->second.second.player) iterator->second.second.player->shouldBeErased = true;
			iterator = clients.erase(iterator);
		} else ++iterator;
	}
	pendingDisconnects.clear();
}

void AngleShooterServer::sendToAll(const OutputBitStream& packet, const std::function<bool(const std::pair<std::unique_ptr<NetworkPair>, PlayerDetails>&)>& predicate) {
    for (const auto& pair : clients | std::views::values) {
        if (!predicate(pair)) continue;
        send(packet, pair.first);
    }
}

void AngleShooterServer::send(OutputBitStream packet, const std::unique_ptr<NetworkPair>& pair) {
	pair->send(packet);
}

sf::UdpSocket& AngleShooterServer::getSocket() {
	return this->listenerSocket;
}