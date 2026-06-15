#pragma once
#include "PlayerState.h"
#include "SpriteAnimator.h" // Intégration de l'animateur par composition

class DeadState : public PlayerState {
public:
    DeadState();
    virtual ~DeadState() = default;

    void update(Player& player, float deltaTime) override;

    void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const override;

private:
    SpriteAnimator m_animator; // L'état possède son propre animateur de mort
    bool m_finished = false;
    bool m_started = false;
};