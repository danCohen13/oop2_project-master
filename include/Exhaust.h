#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteAnimator.h"

class Exhaust {
public:
    Exhaust();

    void update(float deltaTime, const sf::Vector2f& playerPos);

    void draw(sf::RenderWindow& window) const;

    bool isActive() const { return m_active; }

    void setActive(bool active);

    void syncFrame(int playerFrame);

private:
    sf::Sprite     m_sprite;
    SpriteAnimator m_animator;
    bool           m_active = false;

    static constexpr float OFFSET_X = 22.f;
    static constexpr float OFFSET_Y = 80.f;
};