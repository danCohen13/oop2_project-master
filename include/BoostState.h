#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class BoostState : public PlayerState {
public:
    BoostState(float currentX, float distanceInPixels);
    virtual ~BoostState() override = default;

    virtual void update(Player& player, float deltaTime) override;

    // CORRECTION SIGNATURE CONST : Correspondance exacte et stricte avec l'interface PlayerState.h
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

private:
    float m_targetX;
    mutable SpriteAnimator m_speedFlameAnimator;
};