#include "PreCompiledClient.h"
#include "MenuManager.h"

MenuWidget* MenuManager::addWidget(MenuWidget* page) {
	this->widgets.push_back(page);
	return page;
}

MenuPage* MenuManager::addPage(MenuPage* page) {
	this->pages.push_back(page);
	if (this->currentPage == nullptr) this->currentPage = page;
	return page;
}

void MenuManager::tick() {
	for (const auto& widget : this->widgets) widget->tick();
	if (this->currentPage == nullptr) return;
	this->currentPage->tick();
	this->prevView = this->view;
	this->view = Util::lerp(0.1f, this->view, this->currentPage->getView());
}

void MenuManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	const auto oldView = target.getView();
	const auto view = Util::lerp(static_cast<float>(AngleShooterClient::get().tickDelta), this->prevView, this->view);
	target.setView({view.getCenter(), view.getSize()});
	for (const auto& widget : this->widgets) target.draw(*widget, states);
	for (const auto& page : this->pages) target.draw(*page, states);
	target.setView(oldView);
}

void MenuManager::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) this->input(MenuInput::ESCAPE);
		if (keyPressed->scancode == sf::Keyboard::Scan::Enter || keyPressed->scancode == sf::Keyboard::Scan::Space) this->input(MenuInput::PRESS);
		if (keyPressed->scancode == sf::Keyboard::Scan::Up || keyPressed->scancode == sf::Keyboard::Scan::W) this->input(MenuInput::UP);
		if (keyPressed->scancode == sf::Keyboard::Scan::Down || keyPressed->scancode == sf::Keyboard::Scan::S) this->input(MenuInput::DOWN);
		if (keyPressed->scancode == sf::Keyboard::Scan::Left || keyPressed->scancode == sf::Keyboard::Scan::A) this->input(MenuInput::LEFT);
		if (keyPressed->scancode == sf::Keyboard::Scan::Right || keyPressed->scancode == sf::Keyboard::Scan::D) this->input(MenuInput::RIGHT);
	}
}

void MenuManager::input(const MenuInput& input) {
	if (this->currentPage == nullptr) return;
	if (input == MenuInput::ESCAPE) {
		this->currentPage = this->currentPage->getPreviousPage();
	} else {
		if (this->currentPage->getSelectedButton() == nullptr && input == MenuInput::PRESS) {
			this->currentPage = this->currentPage->getPreviousPage();
		} else {
			this->currentPage->input(input);
		}
	}
}

void MenuManager::setCurrentPage(MenuPage* page) {
	this->currentPage = page;
}