#include "PreCompiledClient.h"
#include "ServerListState.h"

const Identifier ServerListState::SERVER_LIST_ID("servers");

ServerListState::ServerListState() {}

void ServerListState::init() {
	gui.clear();

	const auto optionsIp = std::make_shared<Button>();
	optionsIp->setPosition({80.f, 364.f});
	auto ipText = [&](const std::string& ip) { return "Options: " + ip; };
	optionsIp->setText(ipText(OptionsManager::get().getIp()));
	optionsIp->setCallback([this] {
		AngleShooterClient::get().connect(PortedIP(sf::IpAddress::resolve(OptionsManager::get().getIp()).value(), AngleShooterCommon::PORT));
		requestStackClear();
		requestStackPush(GameState::GAME_ID);
	});
	gui.pack(optionsIp);

	const auto localIp = std::make_shared<Button>();
	localIp->setPosition({80.f, 400.f});
	localIp->setText("localhost: 127.0.0.1");
	localIp->setCallback([this] {
		AngleShooterClient::get().connect(PortedIP(sf::IpAddress(127, 0, 0, 1)));
		requestStackClear();
		requestStackPush(GameState::GAME_ID);
	});
	gui.pack(localIp);

	auto packet = NetworkProtocol::SERVER_SCAN->getPacket();
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = AngleShooterClient::get().getSocket().send(packet, sf::IpAddress(255, 255, 255, 255), AngleShooterCommon::PORT);

	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({300.f, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
}

void ServerListState::loadAssets() {}

void ServerListState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MenuState::MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		background.setColor({200, 255, 255, 255});
		Util::centre(background);
		background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		background.setScale({2.f, 2.f});
	});
	auto& texture = AngleShooterClient::get().renderTexture;
	texture.draw(background);
	texture.draw(gui);
}

bool ServerListState::shouldRenderNextState() const {
	return false;
}

bool ServerListState::tick() {
	gui.tick();
	return false;
}

bool ServerListState::handleEvent(const sf::Event& event) {
	if (const auto keybinding = InputManager::get().getSelectedKeybinding()) {
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode != sf::Keyboard::Scan::Escape && keyPressed->scancode != sf::Keyboard::Scan::Enter && keyPressed->scancode != sf::Keyboard::Scan::Space) {
				keybinding->rebind(keyPressed->scancode);
				InputManager::get().setSelectedKeybinding(nullptr);
				const auto selected = gui.getSelectedChild();
				this->init();
				gui.setSelected(selected);
				gui.selectNext();
				gui.selectPrevious();
				return false;
			}
		}
	}
	gui.handleEvent(event);
	return false;
}

Identifier ServerListState::getId() {
	return SERVER_LIST_ID;
}

Identifier ServerListState::getStateId() {
	return SERVER_LIST_ID;
}