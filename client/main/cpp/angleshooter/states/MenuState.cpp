#include "PreCompiledClient.h"
#include "MenuState.h"

#include "ServerListState.h"

const Identifier MenuState::MENU_ID("menu");

void MenuState::init() {
	for (auto x = -1; x <= 1; ++x) {
		for (auto y = -1; y <= 1; ++y) {
			this->menu.addWidget(new MenuWidget({1860.f * x, 1317.f * y}, 1860, Identifier("menu/menu_bg.png")));
		}
	}
	this->menu.addWidget(new MenuWidget({-763, 32}, 461, Identifier("menu/menu_credits_techy.png")));
	this->menu.addWidget(new MenuWidget({0, 0}, 1044, Identifier("menu/menu_main.png")));
	this->menu.addWidget(new MenuWidget({-17, -134}, 508, Identifier("menu/menu_logo.png")));

	const auto pageMain = this->menu.addPage(new MenuPage({{0, -20}, sf::Vector2f{1920, 1080} * .7f}));
	const auto pageCredits = this->menu.addPage(new MenuPage({{-763, 2}, sf::Vector2f{1920, 1080} * .55f}, pageMain));
	const auto pageOptions = this->menu.addPage(new MenuPage({{980, 540}, {1920, 1080}}, pageMain));

	const auto widgetServers = pageMain->addButton(new MenuButton({-27, 88}, 232, Identifier("menu/menu_button_servers.png"), ([this] {

	})));
	const auto widgetCredits = pageMain->addButton(new MenuButton({-278, 96}, 241, Identifier("menu/menu_button_credits.png"), ([this, pageCredits] {
		this->menu.setCurrentPage(pageCredits);
	})), widgetServers, MenuInput::RIGHT);
	const auto widgetOptions = pageMain->addButton(new MenuButton({218, 105}, 231, Identifier("menu/menu_button_options.png"), ([this, pageOptions] {
		this->menu.setCurrentPage(pageOptions);
	})), widgetServers, MenuInput::LEFT);
	const auto widgetExit = pageMain->addButton(new MenuButton({-47, 221}, 320, Identifier("menu/menu_button_exit.png"), ([this] {
		this->requestStackPop();
	})), widgetServers, MenuInput::UP);
	pageMain->addLink(widgetExit, widgetCredits, MenuInput::UP);
	pageMain->addLink(widgetExit, widgetOptions, MenuInput::UP);
	pageMain->addLink(widgetExit, widgetCredits, MenuInput::LEFT);
	pageMain->addLink(widgetExit, widgetOptions, MenuInput::RIGHT);




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