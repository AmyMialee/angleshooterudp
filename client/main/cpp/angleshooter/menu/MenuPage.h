#pragma once
#include "MenuButton.h"

class MenuPage : public sf::Transformable, public sf::Drawable {
	std::vector<MenuButton*> buttons;
	MenuButton* selectedButton = nullptr;
	sf::View view;
	MenuPage* previousPage = nullptr;

public:
	explicit MenuPage(const sf::View& view, MenuPage* previousPage = nullptr);
	void tick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	MenuButton* addButton(MenuButton* button, MenuButton* neighbour = nullptr, MenuInput side = MenuInput::ESCAPE);

	void input(MenuInput input);

	[[nodiscard]] MenuButton* getSelectedButton() const;
	[[nodiscard]] MenuPage* getPreviousPage() const;
	[[nodiscard]] sf::View getView() const;
};
