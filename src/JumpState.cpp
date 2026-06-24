#include "JumpState.h"
#include "Player.h"
#include "WalkState.h" 
#include "Resources.h"

JumpState::JumpState() {}

void JumpState::update(Player& player, float deltaTime) {
    player.applyStandardPhysics(deltaTime, true);

    if (player.getSprite().getPosition().y >= player.getFloorY()) {
        player.changeState(std::make_unique<WalkState>());
        return;
    }

    auto& sprite = player.getSprite();
    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = static_cast<int>(playerTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(playerTex.getSize().y);
    sprite.setTextureRect(sf::IntRect({ 3 * playerWidth, 0 }, { playerWidth, playerHeight }));

    player.getExhaust().setActive(player.isThrusting());
    if (player.isThrusting()) {
        player.getExhaust().syncFrame(3);
    }
}

void JumpState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    exhaust.draw(window);
    window.draw(playerSprite);
}