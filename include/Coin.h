#pragma once
#include "StaticGameObject.h"

class Player;

class Coin : public StaticGameObject {
public:
    explicit Coin(const sf::Vector2f& position);
    virtual ~Coin() override = default;

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void update(float deltaTime) override;
    virtual void collide(Object& other) override;

    virtual void collide(Player& player) override;
    virtual bool isDisposed() const override;

private:
    bool m_isDisposed = false; 
};