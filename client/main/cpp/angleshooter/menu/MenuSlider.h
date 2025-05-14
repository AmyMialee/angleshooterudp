#pragma once

struct MenuSlider : MenuButton {
	std::function<std::string(double)> textCallback;
	std::function<void(double)> constantCallback;
	std::function<void(double)> finalCallback;
	bool pressed = false;
	double value = 0;
	bool forward = false;
	bool backward = false;
	unsigned int textWidth = 72 * 6;
	RectangleShape empty = RectangleShape({428, 116});
	RectangleShape bar = RectangleShape({428, 116});
	sf::Texture textTexture;
	RectangleShape text;

	explicit MenuSlider(sf::Vector2f position = {0, 0}, double value = 0, const std::function<std::string(double)>& textCallback = nullptr, const std::function<void(double)>& finalCallback = nullptr, const std::function<void(double)>& constantCallback = nullptr);
	explicit MenuSlider(sf::Vector2f position = {0, 0}, double value = 0, const std::string& textCallback = "", const std::function<void(double)>& finalCallback = nullptr, const std::function<void(double)>& constantCallback = nullptr);
	void refreshTexture();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void tick(bool selected) override;
	void input(MenuInput input);
	const std::function<std::string(double)>& createTextFunction(const std::string& textFunction);
	MenuSlider* setTextFunction(const std::string& textFunction);
	MenuSlider* setTextFunction(const std::function<std::string(double)>& textFunction);
	MenuSlider* setConstantCallback(const std::function<void(double)>& textCallback);
	MenuSlider* setFinalCallback(const std::function<void(double)>& callback);
	[[nodiscard]] sf::Vector2f getOffset() const override;
};