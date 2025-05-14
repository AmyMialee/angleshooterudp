#pragma once

struct NameTypingButton : MenuButton {
	std::function<std::string(std::string)> textCallback;
	bool pressed = false;
	RectangleShape underline = RectangleShape({428, 116});
	sf::Texture textTexture;
	RectangleShape text;
	std::string inputString;

	explicit NameTypingButton(sf::Vector2f position = {0, 0}, std::string value = "", const std::function<std::string(std::string)>& textCallback = nullptr);
	explicit NameTypingButton(sf::Vector2f position = {0, 0}, const std::string& value = "", const std::string& textCallback = "");
	void refreshTexture();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event);
	static std::function<std::string(std::string)> createTextFunction(const std::string& textFunction);
	NameTypingButton* setTextFunction(const std::string& textFunction);
	NameTypingButton* setTextFunction(const std::function<std::string(std::string)>& textFunction);
	[[nodiscard]] sf::Vector2f getOffset() const override;
};