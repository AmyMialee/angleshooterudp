#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

#include "managers/GameManager.h"

int main(int, char*[]) {
	try {
		AngleShooterClient::get().run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterClient::AngleShooterClient() {
	NetworkProtocol::initialize();
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(false);
	socket.setBlocking(false);
	if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) throw std::runtime_error("Failed to bind to port, no ports are remaining???");
	Logger::info("Client started on port " + std::to_string(socket.getLocalPort()));
	registerPackets();
}

void AngleShooterClient::registerPackets() {
	registerPacket(NetworkProtocol::PING, [this](sf::Packet&, const NetworkPair* sender) {
		Logger::debug("Ping! from " + sender->getPortedIP().toString());
		auto pong = NetworkProtocol::PONG->getPacket();
		send(pong);
	});
	registerPacket(NetworkProtocol::PONG, [this](sf::Packet&, NetworkPair* sender) {
		const auto rtt = sender->stopRoundTripTimer();
		Logger::debug("Pong! from " + sender->getPortedIP().toString() + " in " + Util::toRoundedString(rtt * 1000, 0) + "ms");
	});
	registerPacket(NetworkProtocol::ACK, [this](sf::Packet& packet, NetworkPair* sender) {
		uint32_t sequence;
		packet >> sequence;
		sender->acceptAcknowledgment(sequence);
	});
	registerPacket(NetworkProtocol::CHAT_MESSAGE, [this](sf::Packet& packet, const NetworkPair*) {
		std::string message;
		packet >> message;
		Logger::info(message);
	});
	registerPacket(NetworkProtocol::S2C_INITIAL_SETUP, [this](sf::Packet& packet, const NetworkPair*) {
		Logger::debug("Received Initial Setup Packet");
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
		packet >> this->playerId;
	});
	registerPacket(NetworkProtocol::S2C_BROADCAST_MESSAGE, [this](sf::Packet& packet, const NetworkPair*) {
		std::string message;
		packet >> message;
		Logger::debug("Received Broadcast Message Packet from server: " + message);
	});
	registerPacket(NetworkProtocol::S2C_PLAY_MUSIC, [this](sf::Packet& packet, const NetworkPair*) {
		Identifier id;
		float volume, pitch;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		ClientWorld::get().playMusic(id, volume, pitch);
	});
	registerPacket(NetworkProtocol::S2C_PLAY_SOUND, [this](sf::Packet& packet, const NetworkPair*) {
		Identifier id;
		float volume, pitch, x, y, attenuation;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		packet >> x;
		packet >> y;
		packet >> attenuation;
		ClientWorld::get().playSound(id, volume, pitch, {x, y}, attenuation);
	});
	registerPacket(NetworkProtocol::S2C_PLAY_SOUND_3D, [this](sf::Packet& packet, const NetworkPair*) {
		Identifier id;
		float volume, pitch, x, y, z, attenuation;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		packet >> x;
		packet >> y;
		packet >> z;
		packet >> attenuation;
		ClientWorld::get().playSound3d(id, volume, pitch, {x, y, z}, attenuation);
	});
	registerPacket(NetworkProtocol::S2C_LOAD_MAP, [this](sf::Packet& packet, const NetworkPair*) {
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
	});
	registerPacket(NetworkProtocol::S2C_SPAWN_PLAYER, [this](sf::Packet& packet, const NetworkPair*) {
		const auto player = ClientWorld::get().spawnPlayer(packet);
		packet >> player->isClientPlayer;
		GameManager::get().SCORES.emplace(player->getId(), ScoreEntry{player->name, player->cosmetics, player->score, 0, 0});
		GameManager::get().refreshScores();
	});
	registerPacket(NetworkProtocol::S2C_SPAWN_BULLET, [this](sf::Packet& packet, const NetworkPair*) {
		ClientWorld::get().spawnBullet(packet);
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_INPUT, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		float x, y;
		float firingX, firingY;
		packet >> id;
		packet >> x;
		packet >> y;
		packet >> firingX;
		packet >> firingY;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player input for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->input = {x, y};
			player->firingInput = {firingX, firingY};
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_POSITION_SYNC, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		if (id == this->playerId) {
			packet.clear();
			return;
		}
		float x, y;
		packet >> x >> y;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			entity->setPosition({x, y});
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_BULLET_CHARGE, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		uint16_t charge;
		packet >> charge;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player bullet charge for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->bulletCharge = charge;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_HEALTH, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		uint16_t health;
		packet >> health;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player health for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->health = health;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_DEATH, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player death for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->deathTime = 60;
			player->immunityTime = 120;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_TELEPORT, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		float x, y;
		packet >> x >> y;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player teleport for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->setPosition({x, y});
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_REMOVE_OBJECT, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		auto iterator = ClientWorld::get().gameObjects.begin();
		while (iterator != ClientWorld::get().gameObjects.end()) {
			if (iterator->first == id) {
				iterator = ClientWorld::get().gameObjects.erase(iterator);
				if (const auto it = GameManager::get().SCORES.find(id); it != GameManager::get().SCORES.end()) {
					GameManager::get().SCORES.erase(it);
					GameManager::get().refreshScores();
				}
			} else {
				++iterator;
			}
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_SCORE, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		uint16_t score;
		packet >> score;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player score for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->score = score;
			if (const auto it = GameManager::get().SCORES.find(player->getId()); it != GameManager::get().SCORES.end()) {
				it->second.score = player->score;
			} else {
				GameManager::get().SCORES.emplace(player->getId(), ScoreEntry{player->name, player->cosmetics, player->score, 0, 0});
			}
			GameManager::get().refreshScores();
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_NAME, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		std::string name;
		packet >> name;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player name for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->name = name;
			if (const auto it = GameManager::get().SCORES.find(player->getId()); it != GameManager::get().SCORES.end()) {
				it->second.name = player->name;
				GameManager::get().refreshScores();
			}
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_COSMETICS, [this](sf::Packet& packet, const NetworkPair*) {
		uint16_t id;
		packet >> id;
		PlayerCosmetics cosmetics;
		packet >> cosmetics;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player colour for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->cosmetics = cosmetics;
			if (const auto it = GameManager::get().SCORES.find(player->getId()); it != GameManager::get().SCORES.end()) {
				it->second.cosmetics = cosmetics;
				GameManager::get().refreshScores();
			}
			return;
		}
	});
}

void AngleShooterClient::handlePacket(sf::Packet& packet, NetworkPair* sender) {
	sender->resetTimeout();
	uint8_t packetType;
	packet >> packetType;
	if (PacketIdentifier::fromId(packetType)->isReliable()) {
		uint32_t sequence;
		packet >> sequence;
		if (sequence < sender->getAcknowledgedSequence()) {
			auto ack = NetworkProtocol::ACK->getPacket();
			ack << sequence;
			send(ack);
			Logger::debug("Received redundant sequence: " + std::to_string(sequence) + " from " + sender->getPortedIP().toString());
			return;
		}
		if (!sender->setAcknowledgedSequence(sequence)) {
			Logger::debug("Received premature sequence: " + std::to_string(sequence) + " expected " + std::to_string(sender->getAcknowledgedSequence()) + " from " + sender->getPortedIP().toString());
		} else {
			auto ack = NetworkProtocol::ACK->getPacket();
			ack << sequence;
			send(ack);
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
	Logger::error("Received unknown packet id: " + std::to_string(packetType) + " from " + sender->getPortedIP().toString());
}

void AngleShooterClient::registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler) {
	this->packetHandlers.emplace(packetType->getId(), handler);
	this->packetIds.emplace(packetType->getId(), packetType);
}

void AngleShooterClient::run() {
	Logger::info("Starting AngleShooter Client");
	sf::Sprite splashLogo(TextureHolder::getInstance().get(Identifier("sfml_logo.png")));
	Util::centre(splashLogo);
	splashLogo.setPosition(window.getView().getSize() / 2.f);
	auto splashClock = sf::Clock();
	while (splashClock.isRunning()) {
		const auto percent = splashClock.getElapsedTime().asSeconds() / 4.f;
		if (percent >= 1.f) {
			splashClock.stop();
			break;
		}
		const auto alpha = std::clamp(255 * std::pow(std::sin(Util::toRadians(160 * std::pow(percent, 2))), 2), 0., 255.);
		splashLogo.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
		const auto scale = .4f + .4f * percent;
		splashLogo.setScale({scale, scale});
		window.clear();
		window.draw(splashLogo);
		window.display();
		while (const auto optional = window.pollEvent()) {
			if (!optional.has_value()) continue;
			if (auto event = optional.value(); event.getIf<sf::Event::KeyPressed>()) {
				splashClock.stop();
			} else if (event.is<sf::Event::Closed>()) {
				window.close();
				return;
			}
		}
	}
	MainMenuManager::get().populateMainMenu();
	auto packet = NetworkProtocol::SERVER_SCAN->getPacket();
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = getSocket().send(packet, sf::IpAddress(255, 255, 255, 255), AngleShooterCommon::PORT);
	ClientWorld::get().init();
    std::thread receiverThread(&AngleShooterClient::runReceiver, this);
	sf::Clock deltaClock;
	auto frameTime = 0.;
	auto tickTime = 0.;
	auto secondTime = 0.;
	auto frames = 0.;
	auto ticks = 0.;
	auto loops = 0.;
	while (window.isOpen()) {
		const auto deltaTime = deltaClock.restart().asSeconds();
		tickTime += deltaTime;
		frameTime += deltaTime;
		secondTime += deltaTime;
		InputManager::get().handleInput(window);
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK) + " ticks (" + Util::toRoundedString(tickTime) + " seconds), skipping ahead");
			tickTime = AngleShooterCommon::TIME_PER_TICK;
		}
		while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
			tickTime -= AngleShooterCommon::TIME_PER_TICK;
			AudioManager::get().tick();
			if (onMainMenu) {
				MainMenuManager::get().tick();
			} else {
				GameManager::get().tick();
			}
			++ticks;
		}
		if (frameTime >= OptionsManager::get().getTimePerFrame()) {
			this->tickDelta = tickTime / AngleShooterCommon::TIME_PER_TICK;
			render();
			while (frameTime >= OptionsManager::get().getTimePerFrame()) frameTime -= OptionsManager::get().getTimePerFrame();
			++frames;
		}
		++loops;
		if (secondTime >= .05f) {
			tps = tps * .95 + .05 * (ticks / secondTime);
			fps = fps * .95 + .05 * (frames / secondTime);
			lps = lps * .95 + .05 * (loops / secondTime);
			ticks = 0;
			frames = 0;
			loops = 0;
			secondTime = 0;
		}
		sf::sleep(std::chrono::milliseconds(1));
	}
}

void AngleShooterClient::render() {
	window.clear();
	if (onMainMenu) {
		window.draw(MainMenuManager::get());
	} else {
		window.draw(GameManager::get());
	}
	if (this->debug) {
		auto offset = 0;
		auto fill = [&](const std::string& words) {
			static auto text = sf::Text(FontHolder::getInstance().getDefault(), "", 12);
			text.setString(words);
			text.setFillColor(sf::Color::Black);
			const auto topLeftPos = window.getView().getCenter() - window.getView().getSize() / 2.f;
			text.setPosition(topLeftPos + sf::Vector2f{4.f, 4.f + offset} + sf::Vector2f{1.f, 1.f});
			window.draw(text);
			text.setFillColor(sf::Color::White);
			text.setPosition(topLeftPos + sf::Vector2f{4.f, 4.f + offset});
			window.draw(text);
			offset += 14;
		};
		fill("TPS: " + Util::toRoundedString(tps, 2));
		fill("FPS: " + Util::toRoundedString(fps, 2));
		fill("LPS: " + Util::toRoundedString(lps, 2));
		fill("Sequence: " + std::to_string(this->server ? this->server->getAcknowledgedSequence() : 0));
	}
	window.display();
}

void AngleShooterClient::runReceiver() {
	Logger::info("Starting Client Network Handler");
	while (window.isOpen()) {
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; socket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			if (!sender.has_value()) continue;
			auto receivedPip = PortedIP{.ip= sender.value(), .port= port};
			if (!this->server) {
				if (packet.getDataSize() > 0) {
					if (const auto type = static_cast<const uint8_t*>(packet.getData())[0]; type == NetworkProtocol::SERVER_SCAN->getId()) {
						MainMenuManager::get().addServer(receivedPip);
						continue;
					}
				}
				Logger::warn("Received packet from non-server address: " + receivedPip.toString());
				sleep(sf::milliseconds(64));
				continue;
			}
			handlePacket(packet, this->server);
			continue;
		}
		if (!this->server) {
			sleep(sf::milliseconds(128));
			continue;
		}
		this->server->update();
		if (this->server->shouldDisconnect()) this->disconnect();
		sleep(sf::milliseconds(6));
	}
}

void AngleShooterClient::send(sf::Packet& packet) {
	if (this->server == nullptr) {
		Logger::error("Cannot send packet, not connected to server");
		return;
	}
	this->server->send(packet);
}

void AngleShooterClient::connect(const PortedIP& server) {
	this->server = new NetworkPair(*this, server);
	auto join = NetworkProtocol::C2S_JOIN->getPacket();
	join << OptionsManager::get().getName();
	join << OptionsManager::get().getCosmetics();
	send(join);
	this->onMainMenu = false;
}

void AngleShooterClient::disconnect() {
	Logger::info("Disconnected from server " + this->server->getPortedIP().toString());
	//TODO: Send disconnect packet
	delete(this->server);
	this->server = nullptr;
	this->onMainMenu = true;
}

sf::UdpSocket& AngleShooterClient::getSocket() {
	return this->socket;
}