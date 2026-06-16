#include "BoostState.h"
#include "Player.h"
#include "Resources.h"

BoostState::BoostState(float currentX, float distanceInPixels)
    : m_targetX(currentX + distanceInPixels),
    // RE-DÉFINITION DU SPRITESHEET : Renseigne ici le nombre EXACT de frames horizontales de ta flamme bleue (ex: 4, 5 ou 6).
    m_speedFlameAnimator(Resources::getInstance().getTexture("speedflame"), 4, 0.05f)
{
}

void BoostState::update(Player& player, float deltaTime) {
    if (player.getSprite().getPosition().x >= m_targetX) {
        player.getSprite().setRotation(sf::degrees(0.f)); // Remise droite (SFML 3.0)
        player.stopBoost();
    }
    else {
        player.getSprite().setRotation(sf::degrees(15.f)); // Inclinaison aérodynamique
        m_speedFlameAnimator.update(deltaTime);
    }
}

// CORRECTION SIGNATURE CONST : Application des qualifications const requises
void BoostState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    // 1. Dessin du sprite du joueur (reçu en const)
    window.draw(playerSprite);

    // 2. Dessin de la super-flamme bleue superposée
    sf::Sprite flameSprite(Resources::getInstance().getTexture("speedflame"));
    m_speedFlameAnimator.applyTo(flameSprite);

    auto frameSize = m_speedFlameAnimator.getFrameSize();
    flameSprite.setOrigin({ 0.0f, static_cast<float>(frameSize.y) / 2.0f });

    // Alignement au pixel près basé sur la position du joueur
    flameSprite.setPosition(playerSprite.getPosition() + sf::Vector2f(-45.f, 10.f));
    flameSprite.setRotation(playerSprite.getRotation());

    window.draw(flameSprite);
}