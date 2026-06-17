#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class SuperPowerRunnerState : public PlayerState {
public:
    SuperPowerRunnerState(Player& player);
    virtual ~SuperPowerRunnerState() override = default;

    virtual void update(Player& player, float deltaTime) override;
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

private:
    SpriteAnimator m_animator;
    bool m_gravityReversed;
    bool m_spaceWasPressed;
    float m_verticalVelocity;

    const float GRAVITY_ACCEL = 3500.f; // Gravité plus lourde pour un effet nerveux et agréable
};