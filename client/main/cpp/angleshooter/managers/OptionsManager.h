#pragma once

class OptionsManager final {
	std::string name = "Player";
	std::string ip = "127.0.0.1";
	PlayerCosmetics cosmetics;
	double masterVolume = 1.;
	double musicVolume = 0.;
	double soundVolume = 0.;
	int framesPerSecond = 144;
	double timePerFrame = 1. / framesPerSecond;
	bool onboarded = false;

protected:
	OptionsManager();
	~OptionsManager() = default;

public:
	OptionsManager(const OptionsManager&) = delete;
	void operator=(const OptionsManager&) = delete;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] std::string getIp() const;
	[[nodiscard]] PlayerCosmetics getCosmetics() const;
	[[nodiscard]] double getMasterVolume() const;
	[[nodiscard]] double getMusicVolume() const;
	[[nodiscard]] double getSoundVolume() const;
	[[nodiscard]] float getHue() const;
	[[nodiscard]] bool isOnboarded() const;
	[[nodiscard]] int getFps();
	[[nodiscard]] double getTimePerFrame();
	void setName(const std::string& name);
	void setIp(const std::string& ip);
	void setCharacter(uint8_t character);
	void setCosmetic(uint8_t cosmetic);
	void setColour(sf::Color colour);
	void setCharacter(Identifier* character);
	void setCosmetic(Identifier* cosmetic);
	void setMasterVolume(double volume);
	void setMusicVolume(double volume);
	void setSoundVolume(double volume);
	void setOnboarded(bool onboarded);
	void setFps(int fps);
	void saveToFile();
	void loadFromFile();

	static OptionsManager& get() {
		static OptionsManager instance;
		return instance;
	}
};