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
    const auto& suitTex = Resources::getInstance().getTexture("SuperPowerRunner");

    sf::Vector2f texSize(suitTex.getSize());
    m_suitFrameSize = { texSize.x / 4.f, texSize.y };

    sprite.setTexture(suitTex, false);
    m_animator.applyTo(sprite);

    sf::Vector2f currentPos = sprite.getPosition();
    sprite.setOrigin(m_suitFrameSize / 2.f);
    sprite.setPosition(currentPos + m_suitFrameSize / 2.f);
}

void SuperPowerRunnerState::update(Player& player, float deltaTime) {
    auto& sprite = player.getSprite();

    player.getExhaust().setActive(false);

    if (player.isThrusting() && !m_lastThrustingState) {
        m_gravityInverted = !m_gravityInverted;
        player.setVerticalVelocity(0.f);
    }
    m_lastThrustingState = player.isThrusting();

    float gravityAcceleration = 1300.f;
    float customGravity = gravityAcceleration * (m_gravityInverted ? -1.0f : 1.0f);
    player.setVerticalVelocity(player.getVerticalVelocity() + customGravity * deltaTime);
    sprite.move({ 0.0f, player.getVerticalVelocity() * deltaTime });

    m_animator.update(deltaTime);
    m_animator.applyTo(sprite);

    sprite.setOrigin(m_suitFrameSize / 2.f);
    sprite.setScale({ 1.f, m_gravityInverted ? -1.0f : 1.0f });

    sf::Vector2f pos = sprite.getPosition();

    float floorCenterY = player.getFloorY() + m_suitFrameSize.y / 2.f;
    float ceilingCenterY = player.getCeilingY() + m_suitFrameSize.y / 2.f;

    if (pos.y >= floorCenterY) {
        pos.y = floorCenterY;
        player.setVerticalVelocity(0.f);
    }
    else if (pos.y <= ceilingCenterY) {
        pos.y = ceilingCenterY;
        player.setVerticalVelocity(0.f);
    }

    sprite.setPosition(pos);
}

void SuperPowerRunnerState::handleHit(Player& player) {
    const auto& normalTex = Resources::getInstance().getTexture("player");

    sf::Vector2f normalTexSize(normalTex.getSize());
    sf::Vector2f normalFrameSize = { normalTexSize.x / 4.f, normalTexSize.y };

    auto& sprite = player.getSprite();
    sf::Vector2f centerPos = sprite.getPosition();

    sprite.setTexture(normalTex, false);
    sprite.setOrigin({ 0.f, 0.f });
    sprite.setScale({ 1.f, 1.f });
    sprite.setRotation(sf::degrees(0.f));

    sf::Vector2f topLeftPos = centerPos - normalFrameSize / 2.f;

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