#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Exhaust;

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void update(Player& player, float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const = 0;

    virtual bool isDead()           const { return false; }
    virtual bool isInvincible()     const { return false; }
    virtual bool isSpeedBoosting()   const { return false; }
    virtual bool isSuperPowerRunner() const { return false; }

    virtual void handleHit(Player& player);
};