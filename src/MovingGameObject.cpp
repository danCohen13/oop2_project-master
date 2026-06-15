#include "MovingGameObject.h"

MovingGameObject::MovingGameObject(const sf::Texture& texture, float speed)
    : Object(texture), m_speed(speed) {
}

void MovingGameObject::move(float deltaTime) {
    m_sprite.move({ m_speed * deltaTime, 0.0f });
}

float MovingGameObject::getSpeed() const { return m_speed; }
void MovingGameObject::setSpeed(float speed) { m_speed = speed; }