#pragma once

class TextureHolder final : public ResourceHolder<sf::Texture> {
	std::unique_ptr<sf::Texture> createDefaultTexture();

protected:
	explicit TextureHolder();
	~TextureHolder() = default;

public:
	explicit TextureHolder(const FontHolder&) = delete;
	void operator=(const TextureHolder&) = delete;
	static TextureHolder& getInstance() {
		static TextureHolder instance;
		return instance;
	}
};