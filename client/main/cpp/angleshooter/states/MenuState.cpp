#include "PreCompiledClient.h"
#include "MenuState.h"

#include "ServerListState.h"
#include "../menu/FloatingWidget.h"

const Identifier MenuState::MENU_ID("menu");

void MenuState::init() {

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