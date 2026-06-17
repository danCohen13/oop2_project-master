#include "JumpState.h"
#include "Player.h"
#include "WalkState.h" // <-- REQUIS pour la transition changeState vers WalkState
#include "Resources.h"

JumpState::JumpState() {}

void JumpState::update(Player& player, float deltaTime) {
    // 1. Application de la physique de saut (gravité + impulsion jetpack)
    player.applyStandardPhysics(deltaTime, true);

    // 2. Transition de retour au sol autonome
    if (player.getSprite().getPosition().y >= player.getFloorY()) {
        player.changeState(std::make_unique<WalkState>());
        return;
    }

    // 3. Cadre fixe (règle du dépôt) : Verrouiller les jambes sur la frame de vol (index 3)
    auto& sprite = player.getSprite();
    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = static_cast<int>(playerTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(playerTex.getSize().y);
    sprite.setTextureRect(sf::IntRect({ 3 * playerWidth, 0 }, { playerWidth, playerHeight }));

    // 4. Synchronisation de la flamme uniquement si l'impulsion est active
    player.getExhaust().setActive(player.isThrusting());
    if (player.isThrusting()) {
        player.getExhaust().syncFrame(3);
    }
}

void JumpState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    exhaust.draw(window);
    window.draw(playerSprite);
}