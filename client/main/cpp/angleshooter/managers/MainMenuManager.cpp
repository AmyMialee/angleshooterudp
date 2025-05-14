#include "PreCompiledClient.h"
#include "MainMenuManager.h"

void MainMenuManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->mainMenuManager);
}

bool MainMenuManager::tick() {
	this->mainMenuManager.tick();
	return false;
}

bool MainMenuManager::handleEvent(const sf::Event& event) {
	this->mainMenuManager.handleEvent(event);
	return false;
}

void MainMenuManager::populateMainMenu() {
	for (auto x = -1; x <= 1; ++x) for (auto y = -1; y <= 1; ++y) this->mainMenuManager.addWidget(new MenuWidget({1860.f * x, 1317.f * y}, 1860, Identifier("menu/menu_bg.png")));
	this->mainMenuManager.addWidget(new MenuWidget({-763, 32}, 461, Identifier("menu/menu_credits_techy.png")));
	this->mainMenuManager.addWidget(new MenuWidget({700, 540}, 788, Identifier("menu/menu_options.png")));
	this->mainMenuManager.addWidget(new MenuWidget({-220, 560}, 1046, Identifier("menu/menu_servers.png")));
	this->mainMenuManager.addWidget(new MenuWidget({0, 0}, 1044, Identifier("menu/menu_main.png")));
	const auto pageMain = this->mainMenuManager.addPage(new MenuPage({{0, -20}, sf::Vector2f{1920, 1080} * .7f}));
	const auto pageCredits = this->mainMenuManager.addPage(new MenuPage({{-763, 2}, sf::Vector2f{1920, 1080} * .55f}, pageMain));
	this->optionsPage = this->mainMenuManager.addPage(new MenuPage({{700, 560}, sf::Vector2f{1920, 1080} * .85f}, pageMain));
	this->serverListPage = this->mainMenuManager.addPage(new MenuPage({{-220, 560}, sf::Vector2f{1920, 1080} * .7f}, pageMain));
	pageMain->addWidget(new FloatingWidget({-17, -129}, {-17, -139}, 508, Identifier("menu/menu_logo.png"), 120));
	const auto widgetServers = pageMain->addButton(new MenuButton({-27, 88}, 232, Identifier("menu/menu_button_servers.png"), ([this] {
		this->mainMenuManager.setCurrentPage(this->serverListPage);
	})));
	const auto widgetCredits = pageMain->addButton(new MenuButton({-278, 96}, 241, Identifier("menu/menu_button_credits.png"), ([this, pageCredits] {
		this->mainMenuManager.setCurrentPage(pageCredits);
	})), widgetServers, MenuInput::RIGHT);
	const auto widgetOptions = pageMain->addButton(new MenuButton({218, 105}, 231, Identifier("menu/menu_button_options.png"), ([this] {
		this->mainMenuManager.setCurrentPage(this->optionsPage);
	})), widgetServers, MenuInput::LEFT);
	const auto widgetExit = pageMain->addButton(new MenuButton({-47, 221}, 320, Identifier("menu/menu_button_exit.png"), ([this] {
		AngleShooterClient::get().window.close();
	})), widgetServers, MenuInput::UP);
	pageMain->addLink(widgetCredits, widgetOptions, MenuInput::LEFT);
	pageMain->addLink(widgetExit, widgetCredits, MenuInput::UP);
	pageMain->addLink(widgetExit, widgetOptions, MenuInput::UP);
	pageMain->addLink(widgetExit, widgetCredits, MenuInput::LEFT);
	pageMain->addLink(widgetExit, widgetOptions, MenuInput::RIGHT);
	pageMain->addLink(widgetExit, widgetServers, MenuInput::DOWN);
	pageMain->addLink(widgetExit, widgetCredits, MenuInput::DOWN);
	pageMain->addLink(widgetExit, widgetOptions, MenuInput::DOWN);
	this->populateOptionsPage();
	this->populateServerPage();
}

void MainMenuManager::populateOptionsPage() {
	const auto widgetBack = this->optionsPage->addButton(new TextButton({500, 240}, {200, 50}, "Back", [this] {
		this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
	}));
	const auto sliderMaster = this->optionsPage->addButton(new MenuSlider({800, 700}, OptionsManager::get().getMasterVolume(), "Master Volume", [this](double value) {
		OptionsManager::get().setMasterVolume(value);
	}, [this](double value) {
		AudioManager::get().setMusicVolume(OptionsManager::get().getMusicVolume() * value);
		AudioManager::get().setSoundVolume(OptionsManager::get().getSoundVolume() * value);
	}), widgetBack, MenuInput::UP);
	const auto sliderMusic = this->optionsPage->addButton(new MenuSlider({800, 800}, OptionsManager::get().getMusicVolume(), "Music Volume", [this](double value) {
		OptionsManager::get().setMusicVolume(value);
	}, [this](double value) {
		AudioManager::get().setMusicVolume(value * OptionsManager::get().getMasterVolume());
	}), sliderMaster, MenuInput::UP);
	const auto sliderSound = this->optionsPage->addButton(new MenuSlider({800, 900}, OptionsManager::get().getSoundVolume(), "Sound Volume", [this](double value) {
		OptionsManager::get().setSoundVolume(value);
	}, [this](double value) {
		AudioManager::get().setSoundVolume(value * OptionsManager::get().getMasterVolume());
	}), sliderMusic, MenuInput::UP);
}

void MainMenuManager::populateServerPage() {
	this->serverListPage->clearButtons();
	const auto widgetBack = this->serverListPage->addButton(new TextButton({20, 430}, {200, 50}, "Back", ([this] {
		this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
	})));

	// const auto localIp = std::make_shared<Button>(); TODO: Add a button to the menu
	// localIp->setPosition({80.f, 400.f - 36 * 4});
	// localIp->setText("IP: " + receivedPip.toString());
	// localIp->setCallback([this, receivedPip] {
	// get().connect(receivedPip);
	// StateManager::get().clear();
	// StateManager::get().push(GameState::GAME_ID);
	// });
	// const auto serverListState = dynamic_cast<ServerListState*>(StateManager::get().getCurrentState()->get());
	// serverListState->gui.pack(localIp);
	// Logger::debug("Scanned server: " + receivedPip.toString());
}

void MainMenuManager::addServer(const PortedIP& server) {
	this->servers.push_back(server);
	this->populateServerPage();
}