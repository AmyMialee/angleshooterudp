#pragma once

class MenuManager : public sf::Drawable {
	std::vector<MenuWidget*> widgets;
	std::vector<MenuPage*> pages;
	MenuPage* mainPage = nullptr;
	MenuPage* currentPage = nullptr;
	sf::View prevView = sf::View({0, 0}, {1920, 1080});
	sf::View view = sf::View({0, 0}, {1920, 1080});

public:
	MenuWidget* addWidget(MenuWidget* page);
	MenuPage* addPage(MenuPage* page);
	void tick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event);

	void input(const MenuInput& input);
	void setCurrentPage(MenuPage* page);

	MenuPage* getMainPage();
};