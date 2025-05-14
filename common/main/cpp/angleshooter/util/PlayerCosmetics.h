#pragma once

struct PlayerCosmetics {
	inline static auto HITORI = new Identifier("characterassets/hitori.png");
	inline static auto KITA = new Identifier("characterassets/kita.png");
	inline static auto NIJIKA = new Identifier("characterassets/nijika.png");
	inline static auto RYO = new Identifier("characterassets/ryo.png");
	inline static auto CHARACTERS = std::array{ HITORI, KITA, NIJIKA, RYO };

	inline static auto AMY_HAT = new Identifier("characterassets/cosmetics/amy_hat.png");
	inline static auto COLON3 = new Identifier("characterassets/cosmetics/colon3.png");
	inline static auto CROWN = new Identifier("characterassets/cosmetics/crown.png");
	inline static auto EYEGROWER = new Identifier("characterassets/cosmetics/eyegrower.png");
	inline static auto HAT_OUT_OF_TIME = new Identifier("characterassets/cosmetics/hat_out_of_time.png");
	inline static auto HORNS = new Identifier("characterassets/cosmetics/horns.png");
	inline static auto ISAAC_REFERENCE = new Identifier("characterassets/cosmetics/isaac_reference.png");
	inline static auto LOSER_MASK = new Identifier("characterassets/cosmetics/loser_mask.png");
	inline static auto COSMETICS = std::array{ AMY_HAT, COLON3, CROWN, EYEGROWER, HAT_OUT_OF_TIME, HORNS, ISAAC_REFERENCE, LOSER_MASK};

	sf::Color colour = {0xFF, 0xFF, 0xFF, 0xFF};
	Identifier* character = KITA;
	Identifier* cosmetic = AMY_HAT;

	[[nodiscard]] uint8_t getCharacterIndex() const {
		return getCharacterIndex(*character);
	}

	[[nodiscard]] unsigned char getCosmeticIndex() const {
		return getCosmeticIndex(*cosmetic);
	}

	static uint8_t getCharacterIndex(const Identifier& character) {
		for (uint8_t i = 0; i < CHARACTERS.size(); ++i) {  // NOLINT(bugprone-too-small-loop-variable)
			if (CHARACTERS[i]->getHash() == character.getHash()) return i;
		}
		return 255;
	}

	static uint8_t getCosmeticIndex(const Identifier& cosmetic) {
		for (uint8_t i = 0; i < COSMETICS.size(); ++i) {  // NOLINT(bugprone-too-small-loop-variable)
			if (COSMETICS[i]->getHash() == cosmetic.getHash()) return i;
		}
		return 255;
	}

	static Identifier* getCharacter(uint8_t index) {
		if (index >= CHARACTERS.size()) return &Identifier::empty;
		return CHARACTERS[index];
	}

	static Identifier* getCosmetic(uint8_t index) {
		if (index >= COSMETICS.size()) return &Identifier::empty;
		return COSMETICS[index];
	}

	friend sf::Packet& operator<<(sf::Packet& packet, const PlayerCosmetics& cosmetics);
	friend sf::Packet& operator>>(sf::Packet& packet, PlayerCosmetics& id);
};