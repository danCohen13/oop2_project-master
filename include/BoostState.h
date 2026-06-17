#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class BoostState : public PlayerState {
public:
    BoostState(float currentX, float distanceInPixels);
    virtual ~BoostState() override = default;

    virtual void update(Player& player, float deltaTime) override;
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

    virtual bool isInvincible()   const override { return true; }
    virtual bool isSpeedBoosting() const override { return true; }

private:
    float m_targetX;
    SpriteAnimator m_speedFlameAnimator;
};