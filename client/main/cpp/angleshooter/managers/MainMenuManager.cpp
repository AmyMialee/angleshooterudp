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
	this->playerPreviewWidget = new PlayerPreviewWidget({828, 490}, {828, 500}, OptionsManager::get().getCosmetics(), 240);
	this->optionsPage->addWidget(this->playerPreviewWidget);
	this->populateOptionsPage();
	this->populateServerPage();
}

void MainMenuManager::populateOptionsPage() {
	if (!this->optionsPage) return;
	this->optionsPage->clearButtons();

	const auto widgetBack = this->optionsPage->addButton(new TextButton({500, 240}, {200, 50}, "Back", 28, [this] {
		this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
	}));

	const auto leftHatButton = this->optionsPage->addButton(new MenuButton({746, 462}, 72, Identifier("menu/button_left.png"), [this] {
		OptionsManager::get().setCosmetic(OptionsManager::get().getCosmetics().getCosmeticIndex() + static_cast<uint8_t>(PlayerCosmetics::COSMETICS.size()) - 1);
		this->playerPreviewWidget->setCosmetics(OptionsManager::get().getCosmetics());
	}));
	const auto rightHatButton = this->optionsPage->addButton(new MenuButton({914, 462}, 72, Identifier("menu/button_right.png"), [this] {
		OptionsManager::get().setCosmetic(OptionsManager::get().getCosmetics().getCosmeticIndex() + 1);
		this->playerPreviewWidget->setCosmetics(OptionsManager::get().getCosmetics());
	}));
	const auto leftCharacterButton = this->optionsPage->addButton(new MenuButton({746, 522}, 72, Identifier("menu/button_left.png"), [this] {
		OptionsManager::get().setCharacter(OptionsManager::get().getCosmetics().getCharacterIndex() + static_cast<uint8_t>(PlayerCosmetics::CHARACTERS.size()) - 1);
		this->playerPreviewWidget->setCosmetics(OptionsManager::get().getCosmetics());
	}));
	const auto rightCharacterButton = this->optionsPage->addButton(new MenuButton({914, 522}, 72, Identifier("menu/button_right.png"), [this] {
		OptionsManager::get().setCharacter(OptionsManager::get().getCosmetics().getCharacterIndex() + 1);
		this->playerPreviewWidget->setCosmetics(OptionsManager::get().getCosmetics());
	}));
	const auto sliderHue = this->optionsPage->addButton(new MenuSlider({710, 605}, OptionsManager::get().getHue(), "Colour", [this](double value) {
		OptionsManager::get().setColour(Util::hsvToRgb(static_cast<float>(value) * 360.f, .5f, 1.f));
		this->playerPreviewWidget->setCosmetics(OptionsManager::get().getCosmetics());
	}, [this](double value) {
		auto cosmetics = OptionsManager::get().getCosmetics();
		cosmetics.colour = Util::hsvToRgb(static_cast<float>(value) * 360.f, .5f, 1.f);
		this->playerPreviewWidget->setCosmetics(cosmetics);
	}));

	const auto sliderMaster = this->optionsPage->addButton(new MenuSlider({380, 380}, OptionsManager::get().getMasterVolume(), "Master Volume", [this](double value) {
		OptionsManager::get().setMasterVolume(value);
	}, [this](double value) {
		AudioManager::get().setMusicVolume(OptionsManager::get().getMusicVolume() * value);
		AudioManager::get().setSoundVolume(OptionsManager::get().getSoundVolume() * value);
	}));
	const auto sliderMusic = this->optionsPage->addButton(new MenuSlider({380, 460}, OptionsManager::get().getMusicVolume(), "Music Volume", [this](double value) {
		OptionsManager::get().setMusicVolume(value);
	}, [this](double value) {
		AudioManager::get().setMusicVolume(value * OptionsManager::get().getMasterVolume());
	}));
	const auto sliderSound = this->optionsPage->addButton(new MenuSlider({380, 540}, OptionsManager::get().getSoundVolume(), "Sound Volume", [this](double value) {
		OptionsManager::get().setSoundVolume(value);
	}, [this](double value) {
		AudioManager::get().setSoundVolume(value * OptionsManager::get().getMasterVolume());
	}));

	this->optionsPage->addLink(widgetBack, sliderMaster, MenuInput::DOWN);
	this->optionsPage->addLink(sliderMaster, sliderMusic, MenuInput::DOWN);
	this->optionsPage->addLink(sliderMusic, sliderSound, MenuInput::DOWN);
	this->optionsPage->addLink(sliderMaster, leftHatButton, MenuInput::RIGHT);
	this->optionsPage->addLink(sliderMusic, leftCharacterButton, MenuInput::RIGHT);
	this->optionsPage->addLink(sliderSound, sliderHue, MenuInput::RIGHT);
	this->optionsPage->addLink(sliderMaster, rightHatButton, MenuInput::LEFT);
	this->optionsPage->addLink(sliderMusic, rightCharacterButton, MenuInput::LEFT);
	this->optionsPage->addLink(sliderSound, sliderHue, MenuInput::LEFT);
	this->optionsPage->addLink(widgetBack, leftHatButton, MenuInput::RIGHT);
	this->optionsPage->addLink(widgetBack, rightHatButton, MenuInput::LEFT);
	this->optionsPage->addLink(leftHatButton, rightHatButton, MenuInput::RIGHT);
	this->optionsPage->addLink(leftCharacterButton, rightCharacterButton, MenuInput::RIGHT);
	this->optionsPage->addLink(leftHatButton, leftCharacterButton, MenuInput::DOWN);
	this->optionsPage->addLink(rightHatButton, rightCharacterButton, MenuInput::DOWN);
	this->optionsPage->addLink(leftCharacterButton, sliderHue, MenuInput::DOWN);
	this->optionsPage->addLink(rightCharacterButton, sliderHue, MenuInput::DOWN);
}

void MainMenuManager::populateServerPage() {
	if (!this->serverListPage) return;
	this->serverListPage->clearButtons();
	const auto widgetBack = this->serverListPage->addButton(new TextButton({20, 430}, {200, 50}, "Back", 28, ([this] {
		this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
	})));
	const auto widgetRefresh = this->serverListPage->addButton(new TextButton({20, 530}, {200, 50}, "Refresh", 24, ([this] {
		this->servers.clear();
		auto packet = NetworkProtocol::SERVER_SCAN->getPacket();
		auto status = sf::Socket::Status::Partial;
		while (status == sf::Socket::Status::Partial) status = AngleShooterClient::get().getSocket().send(packet, sf::IpAddress(255, 255, 255, 255), AngleShooterCommon::PORT);
	})));
	const auto widgetLocal = this->serverListPage->addButton(new TextButton({20, 630}, {200, 50}, "localhost", 18, ([this] {
		this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
		AngleShooterClient::get().connect(PortedIP(sf::IpAddress(127, 0, 0, 1)));
	})));
	this->serverListPage->addLink(widgetBack, widgetRefresh, MenuInput::DOWN);
	this->serverListPage->addLink(widgetRefresh, widgetLocal, MenuInput::DOWN);

	for (const auto& server : this->servers) {
		const auto button = this->serverListPage->addButton(new TextButton({-200, 0}, {0, 50}, server.toString(), 18, ([this, server] {
			this->mainMenuManager.setCurrentPage(this->mainMenuManager.getMainPage());
			AngleShooterClient::get().connect(server);
		})), widgetRefresh, MenuInput::LEFT);
		button->setPosition({20.f, 630.f - static_cast<float>(this->servers.size()) * 60.f});
	}
}

void MainMenuManager::addServer(const PortedIP& server) {
	this->servers.push_back(server);
	this->populateServerPage();
}