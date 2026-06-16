#include "SpeedItem.h"
#include "Resources.h"
#include "Player.h"

SpeedItem::SpeedItem(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("Speed"), position, 1)
{
}

void SpeedItem::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite); // Affiche le sprite hérité de la classe Object
}

void SpeedItem::collide(Object& other) {
    other.collide(*this);
}

void SpeedItem::collide(Player& player) {
    if (!m_isDisposed) {
        // Active le boost de 2000 pixels (équivalent à 200 mètres à notre échelle)
        player.activateSpeedBoost(2000.0f);
        m_isDisposed = true; // L'item est marqué pour disparaître
    }
}

bool SpeedItem::isDisposed() const {
    return m_isDisposed;
}