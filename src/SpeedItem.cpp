#include "SpeedItem.h"
#include "Resources.h"
#include "Player.h"

SpeedItem::SpeedItem(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("Speed"), position, 1)
{
}

void SpeedItem::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite); 
}

void SpeedItem::collide(Object& other) {
    other.collide(*this);
}

void SpeedItem::collide(Player& player) {
    if (!m_isDisposed) {
        player.activateSpeedBoost(2000.0f);
        m_isDisposed = true; 
    }
}

bool SpeedItem::isDisposed() const {
    return m_isDisposed;
}