#pragma once

struct PlayerDetails {
	std::string name = "Anon";
	sf::Color colour = {0xFF, 0xFF, 0xFF, 0xFF};
	std::shared_ptr<ServerPlayerEntity> player = nullptr;
};