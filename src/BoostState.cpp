#include "BoostState.h"
#include "Player.h"
#include "Resources.h"

BoostState::BoostState(float currentX, float distanceInPixels)
    : m_targetX(currentX + distanceInPixels),
    // CORRECTION : Ta spritesheet comporte exactement 6 sous-images (684 / 114 = 6)
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

    // Récupération de la taille de la frame convertie nativement en flottants (Zéro static_cast)
    sf::Vector2f frameSize(m_speedFlameAnimator.getFrameSize());

    // Ancrage centré verticalement sur le flanc gauche de la flamme
    flameSprite.setOrigin({ 0.0f, frameSize.y / 2.0f });

    // CORRECTION : Décalage augmenté vers la gauche (-85.f au lieu de -45.f)
    // Ajuste cette valeur si tu veux la coller encore plus en arrière contre le jetpack
    sf::Vector2f precisionOffset = { -85.0f, 10.0f };

    flameSprite.setPosition(playerSprite.getPosition() + precisionOffset);
    flameSprite.setRotation(playerSprite.getRotation());

    window.draw(flameSprite);
}