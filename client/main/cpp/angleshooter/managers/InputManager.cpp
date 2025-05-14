#include "PreCompiledClient.h"
#include "InputManager.h"

InputManager::InputManager() :
	up(new Keybinding(std::make_shared<Identifier>("up"), sf::Keyboard::Scancode::W)),
	down(new Keybinding(std::make_shared<Identifier>("down"), sf::Keyboard::Scancode::S)),
	left(new Keybinding(std::make_shared<Identifier>("left"), sf::Keyboard::Scancode::A)),
	right(new Keybinding(std::make_shared<Identifier>("right"), sf::Keyboard::Scancode::D)),
	fireUp(new Keybinding(std::make_shared<Identifier>("fireup"), sf::Keyboard::Scancode::Up)),
	fireDown(new Keybinding(std::make_shared<Identifier>("firedown"), sf::Keyboard::Scancode::Down)),
	fireLeft(new Keybinding(std::make_shared<Identifier>("fireleft"), sf::Keyboard::Scancode::Left)),
	fireRight(new Keybinding(std::make_shared<Identifier>("fireright"), sf::Keyboard::Scancode::Right))
{
	this->keybindings.push_back(this->up);
	this->keybindings.push_back(this->down);
	this->keybindings.push_back(this->left);
	this->keybindings.push_back(this->right);
	this->keybindings.push_back(this->fireUp);
	this->keybindings.push_back(this->fireDown);
	this->keybindings.push_back(this->fireLeft);
	this->keybindings.push_back(this->fireRight);
}

void InputManager::handleInput(sf::RenderWindow& window) {
	while (const auto optional = window.pollEvent()) {
		if (!optional.has_value()) continue;
		auto event = optional.value();
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			this->onKeyPressed(keyPressed->scancode);
		} else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
			this->onKeyReleased(keyReleased->scancode);
		} else if (event.is<sf::Event::Closed>()) {
			window.close();
			return;
		} else if (const auto* resized = event.getIf<sf::Event::Resized>()) {
			window.setSize({resized->size.x, (resized->size.x * 9) / 16});
		}
		AngleShooterClient::get().onMainMenu ? MainMenuManager::get().handleEvent(event) : GameManager::get().handleEvent(event);
	}
}

void InputManager::onKeyPressed(sf::Keyboard::Scancode code) {
	for (const auto& keybinding : keybindings) if (keybinding->getKey() == code) keybinding->onPressed();
	if (code == sf::Keyboard::Scancode::F3) AngleShooterClient::get().debug = !AngleShooterClient::get().debug;
	if (code == sf::Keyboard::Scancode::F4) AngleShooterClient::get().hitboxes = !AngleShooterClient::get().hitboxes;
}

void InputManager::onKeyReleased(sf::Keyboard::Scancode code) {
	for (const auto& keybinding : keybindings) if (keybinding->getKey() == code) keybinding->onReleased();
}

Keybinding* InputManager::getSelectedKeybinding() {
	return this->selectedKeybinding;
}

void InputManager::setSelectedKeybinding(Keybinding* key) {
	this->selectedKeybinding = key;
}

bool InputManager::isKeyTaken(sf::Keyboard::Scan key) {
	for (const auto& keybinding : keybindings) if (keybinding->getKey() == key) return true;
	return false;
}

std::vector<std::shared_ptr<Keybinding>>& InputManager::getKeybindings() {
	return this->keybindings;
}

Keybinding* InputManager::getUp() {
	return this->up.get();
}

Keybinding* InputManager::getDown() {
	return this->down.get();
}

Keybinding* InputManager::getLeft() {
	return this->left.get();
}

Keybinding* InputManager::getRight() {
	return this->right.get();
}

Keybinding* InputManager::getFireUp() {
	return this->fireUp.get();
}

Keybinding* InputManager::getFireDown() {
	return this->fireDown.get();
}

Keybinding* InputManager::getFireLeft() {
	return this->fireLeft.get();
}

Keybinding* InputManager::getFireRight() {
	return this->fireRight.get();
}