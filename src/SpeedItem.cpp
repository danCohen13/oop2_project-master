#include "SpeedItem.h"
#include "Resources.h"
#include "Player.h"

SpeedItem::SpeedItem(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("Speed"), position, 1)
{
}

void SpeedItem::collide(Object& other) {
    other.collide(*this);
}

void SpeedItem::collide(Player& player) {
    if (!m_isDisposed) {
        // Active le boost de 2000 pixels (équivalent à 200 mètres à notre échelle)
        player.activateSpeedBoost(2000.0f);
        m_isDisposed = true; // L'item disparaît de la carte
    }
}