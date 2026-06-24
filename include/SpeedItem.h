#pragma once
#include "StaticGameObject.h"

class SpeedItem : public StaticGameObject {
public:
    SpeedItem(const sf::Vector2f& position);
    virtual ~SpeedItem() override = default;

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

    virtual bool isDisposed() const override;

private:
    bool m_isDisposed = false; 
};