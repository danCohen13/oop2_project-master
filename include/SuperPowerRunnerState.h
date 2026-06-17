#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class SuperPowerRunnerState : public PlayerState {
public:
    SuperPowerRunnerState(Player& player);
    virtual ~SuperPowerRunnerState() override = default;

    void update(Player& player, float deltaTime) override;
    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

    bool isSuperPowerRunner() const override { return true; }
    bool isInvincible()       const override { return true; }

    void handleHit(Player& player) override;

private:
    bool m_gravityInverted;
    bool m_lastThrustingState;
    SpriteAnimator m_animator;
};