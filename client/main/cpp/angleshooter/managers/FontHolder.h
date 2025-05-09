#pragma once

class FontHolder final : public ResourceHolder<sf::Font> {
	std::unique_ptr<sf::Font> createDefaultFont();

protected:
	explicit FontHolder();
	~FontHolder() = default;

public:
	explicit FontHolder(const FontHolder&) = delete;
	void operator=(const FontHolder&) = delete;
	static FontHolder& getInstance() {
		static FontHolder instance;
		return instance;
	}
};