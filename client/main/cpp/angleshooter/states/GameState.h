#pragma once

class GameState final : public State {
	MenuManager menu;
	bool paused;

public:
	inline static std::unordered_map<uint16_t, ScoreEntry> SCORES;
	static const Identifier GAME_ID;
	GameState() = default;
	void init() override;
	void destroy() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void render() const;
	static void refreshScores();
	bool tick() override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};
