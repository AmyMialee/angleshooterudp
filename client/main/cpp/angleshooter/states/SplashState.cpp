#include "PreCompiledClient.h"
#include "SplashState.h"

const Identifier SplashState::TITLE_ID("splash");
const Identifier SplashState::SPLASH_TEXTURE("sfml_logo.png");
float SplashState::totalTime = 4.f;

void SplashState::completeSplash() {
    this->requestStackPop();
    this->requestStackPush(MenuState::getId());
    if (!OptionsManager::get().isOnboarded()) this->requestStackPush(OnboardingState::getId());
}

void SplashState::init() {
    fadeTime = 0;
}

void SplashState::loadAssets() {
    TextureHolder::getInstance().load(SPLASH_TEXTURE);
}

void SplashState::render(float deltaTime) {
    static sf::Sprite background(TextureHolder::getInstance().get(SPLASH_TEXTURE));
    static std::once_flag flag;
    std::call_once(flag, [&] {
        Util::centre(background);
        background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
    });
    const auto alpha = std::clamp(255 * std::pow(std::sin(Util::toRadians(160 * std::pow(fadeTime / totalTime, 2))), 2), 0., 255.);
    background.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
    const auto scale = .4f + .4f * fadeTime / totalTime;
    background.setScale({scale, scale});
    auto& texture = AngleShooterClient::get().renderTexture;
    texture.draw(background);
}

bool SplashState::shouldRenderNextState() const {
    return false;
}

bool SplashState::tick() {
    fadeTime += static_cast<float>(AngleShooterCommon::TIME_PER_TICK);
    if (fadeTime >= totalTime) completeSplash();
    return false;
}

bool SplashState::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        completeSplash();
    }
    return false;
}

Identifier SplashState::getId() {
    return TITLE_ID;
}

Identifier SplashState::getStateId() {
    return TITLE_ID;
}