#pragma once
#include "PlayerState.h"

class JumpState : public PlayerState {
public:
    JumpState();
    virtual ~JumpState() override = default;

    void update(Player& player, float deltaTime) override;

    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;
};