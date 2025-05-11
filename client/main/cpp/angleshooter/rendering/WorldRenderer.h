#pragma once

class WorldRenderer final {
	sf::FloatRect viewLast = {{0, 0}, {0, 0}};
	sf::FloatRect viewCurrent = {{0, 0}, {0, 0}};
	sf::FloatRect viewTarget = {{0, 0}, {0, 0}};
	std::unordered_map<int, std::function<void(std::shared_ptr<Entity>)>> renderRegistry;

protected:
	WorldRenderer();
	~WorldRenderer() = default;
	
public:
	WorldRenderer(const WorldRenderer&) = delete;
	void operator=(const WorldRenderer&) = delete;
	void tick();
	void render();
	template<typename T> void registerRenderer(const Identifier& id, std::function<void(std::shared_ptr<T>)> renderer);

	static WorldRenderer& get() {
		static WorldRenderer instance;
		return instance;
	}
};