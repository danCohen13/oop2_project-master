#include "PlayerState.h"
#include "Player.h"
#include "DeadState.h"
#include "AudioManager.h"

void PlayerState::handleHit(Player& player) {
    if (isInvincible() || player.isInvincible()) return;

    // Comportement par défaut (Walk / Jump) : Mort immédiate
    player.changeState(std::make_unique<DeadState>());

    auto& sprite = player.getSprite();
    sprite.setOrigin({ 0.f, 0.f });
    sprite.setScale({ 1.f, 1.f });
    sprite.setRotation(sf::degrees(0.f));

    AudioManager::getInstance().playSound("hit");
}