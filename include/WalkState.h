#pragma once
#include "PlayerState.h"

class Exhaust;

class WalkState : public PlayerState {
public:
    WalkState();
    void update(Player& player, float deltaTime) override;

    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override; // <-- Correspondance exacte

private:
    int m_currentFrame;
    float m_frameTimer;
    const float FRAME_DURATION = 0.1f;
};