#pragma once

class SoundHolder final : public ResourceHolder<sf::SoundBuffer> {
	std::unique_ptr<sf::SoundBuffer> createDefaultSound();

protected:
	explicit SoundHolder();
	~SoundHolder() = default;

public:
	explicit SoundHolder(const SoundHolder&) = delete;
	void operator=(const SoundHolder&) = delete;
	static SoundHolder& getInstance() {
		static SoundHolder instance;
		return instance;
	}
};