#pragma once
#include "StaticGameObject.h"

class SpeedItem : public StaticGameObject {
public:
    SpeedItem(const sf::Vector2f& position);
    virtual ~SpeedItem() override = default;

    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;
};