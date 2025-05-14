#pragma once

class MainMenuManager : public sf::Drawable {
	std::vector<PortedIP> servers;
	MenuManager mainMenuManager;
	MenuPage* optionsPage;
	MenuPage* onboardingPage;
	MenuPage* serverListPage;
	PlayerPreviewWidget* onboardingPreviewWidget;
	PlayerPreviewWidget* playerPreviewWidget;

protected:
	explicit MainMenuManager() = default;
	~MainMenuManager() override = default;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool tick();
	bool handleEvent(const sf::Event& event);

	void populateMainMenu();
	void populateOnboardingPage();
	void populateOptionsPage();
	void populateServerPage();
	void addServer(const PortedIP& server);

	explicit MainMenuManager(const InputManager&) = delete;
	void operator=(const MainMenuManager&) = delete;
	static MainMenuManager& get() {
		static MainMenuManager instance;
		return instance;
	}
};
