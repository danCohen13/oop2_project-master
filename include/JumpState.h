#pragma once
#include "PlayerState.h"

class Exhaust; // Forward Declaration

class JumpState : public PlayerState {
public:
    JumpState();
    void update(Player& player, float deltaTime) override;

    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override; // <-- Correspondance exacte

private:
    int m_currentFrame;
    float m_frameTimer;
    const float FRAME_DURATION = 0.1f;
};