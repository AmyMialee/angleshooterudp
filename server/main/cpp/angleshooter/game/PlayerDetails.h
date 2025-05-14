#pragma once

struct PlayerDetails {
	std::string name = "Anon";
	PlayerCosmetics cosmetics;
	std::shared_ptr<ServerPlayerEntity> player = nullptr;
};
