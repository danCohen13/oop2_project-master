#include "Object.h"

Object::Object(const sf::Texture& texture)
    : m_sprite(texture) {
}

void Object::setPosition(const sf::Vector2f& position) {
    m_sprite.setPosition(position);
}

sf::Vector2f Object::getPosition() const {
    return m_sprite.getPosition();
}

sf::FloatRect Object::getGlobalBounds() const {
    return m_sprite.getGlobalBounds();
}

sf::FloatRect Object::getLocalBounds() const {
    return m_sprite.getLocalBounds();
}