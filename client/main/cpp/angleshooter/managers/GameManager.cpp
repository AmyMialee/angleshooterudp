#include "PreCompiledClient.h"
#include "GameManager.h"

void GameManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(target.getDefaultView());
	target.draw(WorldRenderer::get());
	target.setView(target.getDefaultView());
	for (const auto& score : SCORES | std::views::values) {
		static sf::Sprite playerSprite(TextureHolder::getInstance().getDefault());
		static sf::Text text(FontHolder::getInstance().getDefault());
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(playerSprite);
			playerSprite.setScale(sf::Vector2f{16.f, 16.f}.componentWiseDiv({64.f, 64.f}));
			Util::centre(text);
			text.setCharacterSize(48);
			text.setScale({0.5f, 0.5f});
			text.setFillColor(sf::Color::White);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(2.f);
		});
		const auto pos = sf::Vector2f({22, target.getView().getSize().y / 2 - SCORES.size() * (36 / 2) + score.yCurrent * 36.f});
		playerSprite.setPosition(pos);
		playerSprite.setColor(score.cosmetics.colour);
		playerSprite.setTexture(TextureHolder::getInstance().get(*score.cosmetics.character));
		target.draw(playerSprite);
		playerSprite.setTexture(TextureHolder::getInstance().get(*score.cosmetics.cosmetic));
		target.draw(playerSprite);
		text.setString(score.name + ": " + std::to_string(score.score));
		text.setPosition({pos.x + 22, pos.y - 16});
		const auto textColour = sf::Color({static_cast<std::uint8_t>(score.cosmetics.colour.r / 2 + 128), static_cast<std::uint8_t>(score.cosmetics.colour.g / 2 + 128), static_cast<std::uint8_t>(score.cosmetics.colour.b / 2 + 128)});
		text.setFillColor(textColour);
		target.draw(text);
	}
	target.setView(target.getDefaultView());
	if (this->paused) {
		static sf::RectangleShape backgroundShape;
		static std::once_flag flag;
		std::call_once(flag, [&] {
			backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
			backgroundShape.setSize(target.getView().getSize());
		});
		auto& texture = target;
		texture.draw(backgroundShape);
		texture.setView(texture.getDefaultView());
		texture.draw(this->menu);
	}
}

bool GameManager::tick() {
	ClientWorld::get().tick();
	WorldRenderer::get().tick();
	for (auto& score : SCORES | std::views::values) {
		if (std::abs(score.yTarget - score.yCurrent) > 0.01f) {
			score.yLastCurrent = score.yCurrent;
			score.yCurrent += (score.yTarget - score.yCurrent) * 0.1f;
			if (std::abs(score.yTarget - score.yCurrent) <= 0.01f) {
				score.yCurrent = score.yTarget;
				score.yLastCurrent = score.yCurrent;
			}
		}
	}
	if (this->paused) menu.tick();
	return false;
}

void GameManager::refreshScores() {
	for (const auto& entity : ClientWorld::get().getEntities()) {
		if (entity->getEntityType() != PlayerEntity::ID) continue;
		const auto player = dynamic_cast<PlayerEntity*>(entity.get());
		if (auto it = SCORES.find(player->getId()); it != SCORES.end()) {
			it->second.score = player->score;
		} else {
			SCORES.emplace(player->getId(), ScoreEntry{player->name, player->cosmetics, player->score, 0, 0});
		}
	}
	std::vector<std::pair<uint16_t, uint16_t>> scores;
	for (const auto& [first, second] : SCORES) scores.emplace_back(first, second.score);
	std::ranges::sort(scores, [](const auto& a, const auto& b) { return a.second > b.second; });
	for (auto i = 0; i < SCORES.size(); ++i) {
		if (auto it = SCORES.find(scores[i].first); it != SCORES.end()) it->second.yTarget = static_cast<float>(i);
	}
}

bool GameManager::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) if (keyPressed->scancode == sf::Keyboard::Scan::Escape) this->paused = !this->paused;
	if (this->paused) menu.handleEvent(event);
	return false;
}