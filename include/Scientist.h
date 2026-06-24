#pragma once
#include "MovingGameObject.h"
#include "SpriteAnimator.h"   

class Scientist : public MovingGameObject {
public:
    Scientist(const sf::Vector2f& position);
    virtual ~Scientist() override = default;

    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) const override;

    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

private:
    SpriteAnimator m_animator;
    float m_directionTimer = 0.f; 
};