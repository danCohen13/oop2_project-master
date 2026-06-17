#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class WalkState : public PlayerState {
public:
    WalkState();
    virtual ~WalkState() override = default;

    virtual void update(Player& player, float deltaTime) override;
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

private:
    SpriteAnimator m_animator;
};