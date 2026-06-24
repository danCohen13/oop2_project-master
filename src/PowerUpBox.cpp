#include "PowerUpBox.h"
#include "Resources.h"
#include "Player.h"

PowerUpBox::PowerUpBox(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("PowerUpBox"), position, 1),
    m_isDisposed(false) 
{
}

void PowerUpBox::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void PowerUpBox::collide(Object& other) {
    other.collide(*this); 
}

void PowerUpBox::collide(Player& player) {
    if (m_isDisposed) return;

    m_isDisposed = true; 

    player.activateSuperPowerRunner();
}