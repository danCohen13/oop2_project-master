#pragma once
#include "Object.h"

class MovingGameObject : public Object {
public:
    MovingGameObject(const sf::Texture& texture, float speed);
    virtual ~MovingGameObject() = default;

    virtual void move(float deltaTime);
    virtual void update(float deltaTime) override = 0;

    float getSpeed() const;
    void setSpeed(float speed);

protected:
    float m_speed;
};

