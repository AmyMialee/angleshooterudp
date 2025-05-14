#pragma once

class WorldRenderer final : public sf::Drawable {
	sf::View viewLast = {{0, 0}, {1920, 1080}};
	sf::View viewCurrent = {{0, 0}, {1920, 1080}};
	std::unordered_map<int, std::function<void(sf::RenderTarget& target, std::shared_ptr<Entity>)>> renderRegistry;

protected:
	WorldRenderer();
	~WorldRenderer() override = default;
	
public:
	WorldRenderer(const WorldRenderer&) = delete;
	void operator=(const WorldRenderer&) = delete;
	void tick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	template<typename T> void registerRenderer(const Identifier& id, std::function<void(sf::RenderTarget& target, std::shared_ptr<T>)> renderer);

	static WorldRenderer& get() {
		static WorldRenderer instance;
		return instance;
	}
};