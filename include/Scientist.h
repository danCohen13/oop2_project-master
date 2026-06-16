#pragma once
#include "StaticGameObject.h"

class Scientist : public StaticGameObject {
public:
    Scientist(const sf::Vector2f& position);
    virtual ~Scientist() override = default;

    virtual void draw(sf::RenderWindow& window) const override;

    // Désactivation complète de toutes les interactions de collision
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;
};