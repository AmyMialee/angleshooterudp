#include "PreCompiledClient.h"
#include "MenuState.h"

#include "ServerListState.h"

const Identifier MenuState::MENU_ID("menu");

void MenuState::init() {
	this->menu.addWidget(new MenuWidget({0, 0}, 1860, Identifier("menu/menu_bg.png")));
	this->menu.addWidget(new MenuWidget({0, 0}, 1044, Identifier("menu/menu_main.png")));
	this->menu.addWidget(new MenuWidget({-17, -154}, 508, Identifier("menu/menu_logo.png")));

	const auto pageMain = this->menu.addPage(new MenuPage({{0, 0}, {1920, 1080}}));
	const auto pageOptions = this->menu.addPage(new MenuPage({{980, 540}, {1920, 1080}}, pageMain));

	const auto widgetServers = pageMain->addButton(new MenuButton({-27, 98}, 232, Identifier("menu/menu_button_servers.png"), ([this] {

	})));
	const auto widgetCredits = pageMain->addButton(new MenuButton({-278, 91}, 241, Identifier("menu/menu_button_credits.png"), ([this] {

	})), widgetServers, MenuInput::RIGHT);
	const auto widgetOptions = pageMain->addButton(new MenuButton({218, 116}, 231, Identifier("menu/menu_button_options.png"), ([this, pageOptions] {
		this->menu.setCurrentPage(pageOptions);
	})), widgetServers, MenuInput::LEFT);
	const auto widgetExit = pageMain->addButton(new MenuButton({-32, 217}, 320, Identifier("menu/menu_button_exit.png"), ([this] {
		this->requestStackPop();
	})), widgetServers, MenuInput::UP);

	const auto widgetBack = pageOptions->addButton(new MenuButton({980 + 100, 400}, {200, 50}, Identifier("menu/menu_button_back"), ([this, pageMain] {
		this->menu.setCurrentPage(pageMain);
	})));

	AudioManager::get().playMusic(Identifier("backgroundmusic.ogg"));
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->menu);
}

bool MenuState::tick() {
	this->menu.tick();
	return false;
}

bool MenuState::handleEvent(const sf::Event& event) {
	this->menu.handleEvent(event);
	return false;
}

Identifier MenuState::getId() {
	return MENU_ID;
}

Identifier MenuState::getStateId() {
	return MENU_ID;
}