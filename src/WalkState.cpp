#include "WalkState.h"
#include "Player.h"
#include "JumpState.h"
#include "Resources.h"

WalkState::WalkState()
    : m_animator(Resources::getInstance().getTexture("player"), 4, 0.1f) {
}

void WalkState::update(Player& player, float deltaTime) {
    player.applyStandardPhysics(deltaTime, true);

    if (player.getSprite().getPosition().y < player.getFloorY()) {
        player.changeState(std::make_unique<JumpState>());
        return;
    }

    // RÈGLE GITHUB : Au sol, le Jetpack est éteint
    player.getExhaust().setActive(false);

    m_animator.update(deltaTime);
    m_animator.applyTo(player.getSprite());
}

void WalkState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    window.draw(playerSprite);
}