#include "SuperPowerRunnerState.h"
#include "Player.h"
#include "WalkState.h"
#include "JumpState.h"
#include "Resources.h"

SuperPowerRunnerState::SuperPowerRunnerState(Player& player)
    : m_gravityInverted(false),
    m_lastThrustingState(false),
    m_animator(Resources::getInstance().getTexture("SuperPowerRunner"), 4, 0.1f)
{
    auto& sprite = player.getSprite();
    const sf::Texture& suitTex = Resources::getInstance().getTexture("SuperPowerRunner");
    int playerWidth = static_cast<int>(suitTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(suitTex.getSize().y);

    sprite.setTexture(suitTex, false);
    m_animator.applyTo(sprite);

    // Déplacement fluide des coordonnées vers le centre pour éviter toute téléportation à l'activation
    sf::Vector2f currentPos = sprite.getPosition();
    sprite.setOrigin({ static_cast<float>(playerWidth) / 2.f, static_cast<float>(playerHeight) / 2.f });
    sprite.setPosition({ currentPos.x + playerWidth / 2.f, currentPos.y + playerHeight / 2.f });
}

void SuperPowerRunnerState::update(Player& player, float deltaTime) {
    auto& sprite = player.getSprite();
    const sf::Texture& suitTex = Resources::getInstance().getTexture("SuperPowerRunner");
    int playerWidth = static_cast<int>(suitTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(suitTex.getSize().y);

    // Le réacteur normal reste éteint en mode gravité
    player.getExhaust().setActive(false);

    // 1. Détection et inversion INSTANTANÉE de la gravité
    if (player.isThrusting() && !m_lastThrustingState) {
        m_gravityInverted = !m_gravityInverted;

        // CORRECTION DE LA LATENCE : Remise à zéro de la vitesse pour supprimer l'effet de fronde/flottement
        player.setVerticalVelocity(0.f);
    }
    m_lastThrustingState = player.isThrusting();

    // 2. Physique de gravité nerveuse (Accélération fixée à 1300.f pour un ressenti très réactif)
    float gravityAcceleration = 1300.f;
    float customGravity = gravityAcceleration * (m_gravityInverted ? -1.0f : 1.0f);
    player.setVerticalVelocity(player.getVerticalVelocity() + customGravity * deltaTime);
    sprite.move({ 0.0f, player.getVerticalVelocity() * deltaTime });

    // 3. Animation des frames
    m_animator.update(deltaTime);
    m_animator.applyTo(sprite);

    // 4. RETOURNEMENT VISUEL IMMÉDIAT (Appliqué en continu dans les airs sans attendre l'impact)
    sprite.setOrigin({ static_cast<float>(playerWidth) / 2.f, static_cast<float>(playerHeight) / 2.f });
    sprite.setScale({ 1.f, m_gravityInverted ? -1.0f : 1.0f });

    // 5. CORRECTION DES MESURES ET SÉCURISATION DES BORDURES DE L'ÉCRAN
    sf::Vector2f pos = sprite.getPosition();

    // Alignements parfaits calculés à partir du centre de masse de la boîte de collision
    float floorCenterY = player.getFloorY() + static_cast<float>(playerHeight) / 2.f;
    float ceilingCenterY = player.getCeilingY() + static_cast<float>(playerHeight) / 2.f;

    // Clamping strict pour interdire toute sortie d'écran
    if (pos.y >= floorCenterY) {
        pos.y = floorCenterY;
        player.setVerticalVelocity(0.f); // Stoppe l'accélération contre le sol
    }
    else if (pos.y <= ceilingCenterY) {
        pos.y = ceilingCenterY;
        player.setVerticalVelocity(0.f); // Stoppe l'accélération contre le toit
    }

    sprite.setPosition(pos);
}

void SuperPowerRunnerState::handleHit(Player& player) {
    const sf::Texture& normalTex = Resources::getInstance().getTexture("player");
    int normalWidth = static_cast<int>(normalTex.getSize().x) / 4;
    int normalHeight = static_cast<int>(normalTex.getSize().y);

    auto& sprite = player.getSprite();
    sf::Vector2f centerPos = sprite.getPosition();

    // Restauration du comportement et de la géométrie du joueur standard
    sprite.setTexture(normalTex, false);
    sprite.setOrigin({ 0.f, 0.f });
    sprite.setScale({ 1.f, 1.f });
    sprite.setRotation(sf::degrees(0.f));

    // Reconversion des coordonnées (depuis le repère central vers le repère top-left 0,0)
    sf::Vector2f topLeftPos = { centerPos.x - normalWidth / 2.f, centerPos.y - normalHeight / 2.f };

    // Garde-fous anti-clipping lors du changement brusque de texture
    if (topLeftPos.y > player.getFloorY()) topLeftPos.y = player.getFloorY();
    if (topLeftPos.y < player.getCeilingY()) topLeftPos.y = player.getCeilingY();

    sprite.setPosition(topLeftPos);
    player.setVerticalVelocity(0.f);

    if (sprite.getPosition().y >= player.getFloorY()) {
        player.changeState(std::make_unique<WalkState>());
    }
    else {
        player.changeState(std::make_unique<JumpState>());
    }
}

void SuperPowerRunnerState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    window.draw(playerSprite);
}