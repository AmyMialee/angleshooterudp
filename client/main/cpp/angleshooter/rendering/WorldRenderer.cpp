#include "PreCompiledClient.h"
#include "WorldRenderer.h"

#include "../game/ClientWorld.h"

WorldRenderer::WorldRenderer() {
	registerRenderer<PlayerEntity>(PlayerEntity::ID, [this](sf::RenderTarget& target, const std::shared_ptr<PlayerEntity>& player) {
		if (player->deathTime > 0) return;
		const auto deltaTime = static_cast<float>(AngleShooterClient::get().tickDelta);
		static sf::Sprite playerSprite(TextureHolder::getInstance().getDefault(), sf::IntRect({0, 0}, {64, 64}));
		static sf::Sprite weaponSprite(TextureHolder::getInstance().get(Identifier("misc/gun_small.png")), sf::IntRect({0, 0}, {64, 64}));
		static sf::Text text(FontHolder::getInstance().getDefault());
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(playerSprite);
			const auto bounds = weaponSprite.getLocalBounds();
			weaponSprite.setOrigin({bounds.size.x * 1.25f, std::floor(bounds.position.y + bounds.size.y / 2)});
			weaponSprite.setScale({0.25f, 0.25f});
			playerSprite.setScale({0.25f, 0.25f});
			text.setCharacterSize(48);
			text.setScale({0.125f, 0.125f});
			text.setFillColor(sf::Color::White);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(2.f);
		});
		const auto pos = player->getPosition() + player->getVelocity() * deltaTime;
		playerSprite.setPosition(pos);
		playerSprite.setRotation(player->getRotation());
		playerSprite.setColor(player->cosmetics.colour);
		playerSprite.setTexture(TextureHolder::getInstance().get(*player->cosmetics.character));
		target.draw(playerSprite);
		playerSprite.setTexture(TextureHolder::getInstance().get(*player->cosmetics.cosmetic));
		target.draw(playerSprite);
		if (player->immunityTime > 0) {
			auto circle = sf::CircleShape(player->getScale().x / 2.f + 2);
			circle.setPosition(pos - player->getScale() / 2.f - sf::Vector2f{2.f, 2.f});
			circle.setFillColor(sf::Color(0, 255, 255, 120));
			target.draw(circle);
		}
		if (const auto clientPlayer = dynamic_cast<ClientPlayerEntity*>(player.get()); clientPlayer != nullptr) {
			weaponSprite.setPosition(pos);
			if (std::abs(clientPlayer->weaponRotation.asDegrees()) > 90) {
				weaponSprite.setScale({-0.25f, -0.25f});
			} else {
				weaponSprite.setScale({-0.25f, 0.25f});
			}
			weaponSprite.setRotation(clientPlayer->weaponRotation);
			weaponSprite.setColor(player->cosmetics.colour);
			target.draw(weaponSprite);
		}
		const auto bound = player->getBoundingBox();
		sf::RectangleShape shape;
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(0.5f);
		shape.setPosition(bound.position + player->getVelocity() * deltaTime + sf::Vector2f{0, bound.size.y + 2});
		shape.setSize({bound.size.x, 2});
		shape.setFillColor(sf::Color::Black);
		target.draw(shape);
		shape.setSize({bound.size.x * (static_cast<float>(player->health) / 8.f), 2});
		shape.setFillColor(sf::Color::Green);
		target.draw(shape);
		if (player->immunityTime <= 0 && player->bulletCharge < AngleShooterCommon::MAX_BULLETS) {
			shape.setPosition(bound.position + player->getVelocity() * deltaTime + sf::Vector2f{0, bound.size.y + 4.5f});
			shape.setSize({bound.size.x, 2});
			shape.setFillColor(sf::Color::Black);
			target.draw(shape);
			shape.setSize({bound.size.x * (static_cast<float>(player->bulletCharge) / AngleShooterCommon::MAX_BULLETS), 2});
			shape.setFillColor(sf::Color::Cyan);
			target.draw(shape);
		}
		text.setString(player->name);
		text.setPosition({pos.x - text.getGlobalBounds().size.x / 2, pos.y - 16});
		const auto textColour = sf::Color({static_cast<std::uint8_t>(player->cosmetics.colour.r / 2 + 128), static_cast<std::uint8_t>(player->cosmetics.colour.g / 2 + 128), static_cast<std::uint8_t>(player->cosmetics.colour.b / 2 + 128)});
		text.setFillColor(textColour);
		target.draw(text);
	});
	registerRenderer<BulletEntity>(BulletEntity::ID, [this](sf::RenderTarget& target, const std::shared_ptr<BulletEntity>& bullet) {
		const auto deltaTime = static_cast<float>(AngleShooterClient::get().tickDelta);
		static sf::Sprite bulletCore(TextureHolder::getInstance().get(Identifier("bullet_core.png")));
		static sf::Sprite bulletRing(TextureHolder::getInstance().get(Identifier("bullet_ring.png")));
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(bulletCore);
			Util::centre(bulletRing);
			const auto textureSizeCore = bulletCore.getTexture().getSize();
			const auto textureSizeRing = bulletRing.getTexture().getSize();
			bulletCore.setScale(bullet->getScale().componentWiseDiv({static_cast<float>(textureSizeCore.x), static_cast<float>(textureSizeCore.y)}));
			bulletRing.setScale(bullet->getScale().componentWiseDiv({static_cast<float>(textureSizeRing.x), static_cast<float>(textureSizeRing.y)}));
		});
		const auto pos = bullet->getPosition() + bullet->getVelocity() * deltaTime;
		bulletCore.setPosition(pos);
		bulletRing.setPosition(pos);
		bulletRing.setColor(bullet->colour);
		target.draw(bulletCore);
		target.draw(bulletRing);
	});
}

void WorldRenderer::tick() {
	this->viewLast = this->viewCurrent;
	if (!ClientWorld::get().gameObjects.contains(AngleShooterClient::get().playerId)) return;
	const auto player = ClientWorld::get().gameObjects[AngleShooterClient::get().playerId];
	const sf::View viewTarget = {player->getPosition(), sf::Vector2f{1920, 1080} * 0.25f};
	viewCurrent = Util::lerp(0.1f, viewCurrent, viewTarget);
}

void WorldRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	const auto delta = AngleShooterClient::get().tickDelta;
	const auto view = Util::lerp(static_cast<float>(delta), viewLast, viewCurrent);
	target.setView(view);
	if (ClientWorld::get().mapRenderer != nullptr) target.draw(*ClientWorld::get().mapRenderer);
	for (const auto entity : ClientWorld::get().getEntities()) {
		if (!entity) continue;
		if (auto renderer = renderRegistry.find(entity->getEntityType().getHash()); renderer != renderRegistry.end()) renderer->second(target, entity);
	}
	if (AngleShooterClient::get().hitboxes) {
		sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		for (const auto entity : ClientWorld::get().getEntities()) {
			const auto rect = entity->getBoundingBox();
			shape.setPosition(rect.position);
			shape.setSize(rect.size);
			target.draw(shape);
			const auto pos = entity->getPosition();
			const auto rotation = entity->getRotation().asRadians();
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = pos;
			line[0].color = sf::Color::Red;
			line[1].position = pos + sf::Vector2f(std::cos(rotation) * 16, std::sin(rotation) * 16);
			line[1].color = sf::Color::Red;
			target.draw(line);
		}
	}
	// static auto bloomProcessing = BloomProcessing();
	// bloomProcessing.apply(target, target);
}

template<typename T> void WorldRenderer::registerRenderer(const Identifier& id, std::function<void(sf::RenderTarget&, std::shared_ptr<T>)> renderer) {
    renderRegistry[id.getHash()] = [renderer](sf::RenderTarget& target, const std::shared_ptr<Entity>& entity) {
        renderer(target, std::static_pointer_cast<T>(entity));
    };
}