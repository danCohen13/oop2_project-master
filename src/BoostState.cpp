#include "BoostState.h"
#include "Player.h"
#include "Resources.h"

BoostState::BoostState(float currentX, float distanceInPixels)
    : m_targetX(currentX + distanceInPixels),
    m_speedFlameAnimator(Resources::getInstance().getTexture("speedflame"), 6, 0.05f)
{
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

    const sf::Texture& flameTex = Resources::getInstance().getTexture("speedflame");
    sf::Sprite flameSprite(flameTex);
    m_speedFlameAnimator.applyTo(flameSprite);

    sf::Vector2f frameSize(m_speedFlameAnimator.getFrameSize());

    flameSprite.setOrigin({ 0.0f, frameSize.y / 2.0f });

    sf::Vector2f precisionOffset = { 25.0f, 10.0f };

    flameSprite.setPosition(playerSprite.getPosition() + precisionOffset);
    flameSprite.setRotation(playerSprite.getRotation());

    window.draw(flameSprite);
}

std::vector<sf::IntRect> buildSpeedFlameFrames(const sf::Texture& texture) {
    const int h = static_cast<int>(texture.getSize().y);
    return {
        sf::IntRect({0,   0}, {192, h}),
        sf::IntRect({192, 0}, {190, h}),
        sf::IntRect({382, 0}, {174, h})
    };
}