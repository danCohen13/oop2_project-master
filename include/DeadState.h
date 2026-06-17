#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h"

class DeadState : public PlayerState {
public:
    DeadState();
    virtual ~DeadState() override = default;

    void update(Player& player, float deltaTime) override;

    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

    virtual bool isDead() const override { return true; }

private:
    SpriteAnimator m_animator;
    bool m_finished;
    bool m_started;
};