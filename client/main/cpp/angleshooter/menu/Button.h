#pragma once

class Button : public Widget {
public:
	inline static auto defaultTexture = Identifier("buttonnormal.png");
	inline static auto selectedTexture = Identifier("buttonselected.png");
	inline static auto pressedTexture = Identifier("buttonpressed.png");
	inline static auto buttonSound = Identifier("ui_button_click.ogg");

	typedef std::shared_ptr<Button> Pointer;
	typedef std::function<void()> Callback;

	Button();
	void setCallback(const Callback& callback);
	void setText(const std::string& text);
	virtual void setToggle(bool toggle);

	[[nodiscard]] bool canBeSelected() const override;
	void setSelected(bool selected) override;
	void setPressed(bool pressed) override;

private:
	Callback callback;
	const sf::Texture& defaultTextureRef;
	const sf::Texture& selectedTextureRef;
	const sf::Texture& pressedTextureRef;
	sf::Sprite sprite;
	sf::Text text;
	bool isToggle;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
