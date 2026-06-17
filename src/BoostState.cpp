#include "BoostState.h"
#include "Player.h"
#include "Resources.h"

BoostState::BoostState(float currentX, float distanceInPixels)
    : m_targetX(currentX + distanceInPixels),
    m_speedFlameAnimator(Resources::getInstance().getTexture("speedflame"), 3, 0.05f) {
}

void BoostState::update(Player& player, float deltaTime) {
    if (player.getSprite().getPosition().x >= m_targetX) {
        player.getSprite().setRotation(sf::degrees(0.f));
        player.stopBoost();
    }
    else {
        player.getSprite().setRotation(sf::degrees(20.f));
        m_speedFlameAnimator.update(deltaTime);
    }
}

void BoostState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    window.draw(playerSprite);

    // SFML 3.0 FIX: Allocation sécurisée avec Texture incluse d'office
    const sf::Texture& flameTex = Resources::getInstance().getTexture("speedflame");
    sf::Sprite flameSprite(flameTex);
    m_speedFlameAnimator.applyTo(flameSprite);

    auto frameSize = m_speedFlameAnimator.getFrameSize();
    flameSprite.setOrigin({ 0.0f, static_cast<float>(frameSize.y) / 2.0f });

    flameSprite.setPosition(playerSprite.getPosition() + sf::Vector2f(-45.f, 10.f));
    flameSprite.setRotation(playerSprite.getRotation());

    window.draw(flameSprite);
}