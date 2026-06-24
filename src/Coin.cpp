#include "Coin.h"
#include "Resources.h"

Coin::Coin(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("Coin"), position, 8),
    m_isDisposed(false)
{
}

void Coin::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void Coin::update(float deltaTime) {
    StaticGameObject::update(deltaTime);
}

void Coin::collide(Object& other) { other.collide(*this); }
void Coin::collide(Player&) { m_isDisposed = true; }
bool Coin::isDisposed() const { return m_isDisposed; }