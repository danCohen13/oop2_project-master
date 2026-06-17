#include "PowerUpBox.h"
#include "Resources.h"
#include "Player.h"

PowerUpBox::PowerUpBox(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("PowerUpBox"), position, 1),
    m_isDisposed(false) // Initialisation à faux
{
}

void PowerUpBox::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void PowerUpBox::collide(Object& other) {
    other.collide(*this); // Double Dispatch polymorphique
}

void PowerUpBox::collide(Player& player) {
    if (m_isDisposed) return;

    m_isDisposed = true; // CORRECTION : Marque l'objet pour destruction (comme Coin)

    // Déclenchement de la transformation du joueur !
    player.activateSuperPowerRunner();
}