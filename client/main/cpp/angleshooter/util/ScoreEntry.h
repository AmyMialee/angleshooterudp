#pragma once

class ScoreEntry {
public:
	std::string name;
	PlayerCosmetics cosmetics;
	uint16_t score;
	float yLastCurrent;
	float yCurrent;
	float yTarget;
	ScoreEntry(std::string name, const PlayerCosmetics& cosmetics, uint16_t score, float yLastCurrent, float yCurrent) : name(std::move(name)), cosmetics(cosmetics), score(score), yLastCurrent(yLastCurrent), yCurrent(yCurrent), yTarget(yCurrent) {}
};