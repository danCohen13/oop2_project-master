#include "Scientist.h"
#include "Resources.h"

Scientist::Scientist(const sf::Vector2f& position)
    : StaticGameObject(
        Resources::getInstance().getTexture("Scientist"),
        position,
        // Découpe dynamique basée sur des proportions de frames carrées
        static_cast<int>(Resources::getInstance().getTexture("Scientist").getSize().x /
            Resources::getInstance().getTexture("Scientist").getSize().y)
    )
{
    // Ajustement de l'origine au centre-bas pour que les pieds de l'animation touchent le sol
    auto textureSize = Resources::getInstance().getTexture("Scientist").getSize();
    int frames = textureSize.x / textureSize.y;
    float frameWidth = static_cast<float>(textureSize.x) / frames;
    float frameHeight = static_cast<float>(textureSize.y);

    m_sprite.setOrigin({ frameWidth / 2.0f, frameHeight });
}

// Implémentation du rendu du sprite du scientifique
void Scientist::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

// Implémentations vides = l'objet est traversé sans aucune interaction physique
void Scientist::collide(Object&) {}
void Scientist::collide(Player&) {}