#pragma once

class MenuState final : public State {
	MenuManager menu;
	std::vector<MenuWidget*> widgets;

public:
	static const Identifier MENU_ID;
	void init() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool tick() override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};