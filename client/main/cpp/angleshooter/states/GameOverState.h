#pragma once

class GameOverState final : public State {
	WidgetContainer gui;

public:
	static const Identifier GAME_OVER_ID;
	void init() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick() override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};