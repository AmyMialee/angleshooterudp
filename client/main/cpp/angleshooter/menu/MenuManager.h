#pragma once

class MenuManager : public sf::Drawable {
	std::vector<MenuWidget*> widgets;
	std::vector<MenuPage*> pages;
	MenuPage* currentPage = nullptr;
	sf::View prevView;
	sf::View view;

public:
	MenuWidget* addWidget(MenuWidget* page);
	MenuPage* addPage(MenuPage* page);
	void tick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event);

	void input(const MenuInput& input);
	void setCurrentPage(MenuPage* page);
};