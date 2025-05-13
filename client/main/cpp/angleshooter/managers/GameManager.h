#pragma once

class GameManager : public sf::Drawable {
	MenuManager menu;
	bool paused = false;

protected:
	explicit GameManager() = default;
	~GameManager() override = default;

public:
	std::unordered_map<uint16_t, ScoreEntry> SCORES;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool tick();
	bool handleEvent(const sf::Event& event);

	void refreshScores();
	void init();
	void destroy();

	explicit GameManager(const InputManager&) = delete;
	void operator=(const GameManager&) = delete;
	static GameManager& get() {
		static GameManager instance;
		return instance;
	}
};