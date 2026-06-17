#include "Scientist.h"
#include "Resources.h"

Scientist::Scientist(const sf::Vector2f& position)
    : MovingGameObject(Resources::getInstance().getTexture("Scientist"), -60.f), // Vitesse initiale vers la gauche
    m_animator(Resources::getInstance().getTexture("Scientist"), 3)             // 3 frames de spritesheet
{
    // Applique la première frame pour sécuriser la hitbox
    m_animator.applyTo(m_sprite);

    // Récupération de la taille de la frame pour l'origine (pieds au sol)
    sf::Vector2i frameSize = m_animator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(frameSize.x) / 2.0f, static_cast<float>(frameSize.y) });
    m_sprite.setPosition(position);

    // Comme il commence vers la gauche, on retourne le sprite horizontalement dès le départ
    m_sprite.setScale({ -1.f, 1.f });
}

void Scientist::update(float deltaTime) {
    // 1. Mise à jour des frames de l'animation de marche
    if (m_animator.update(deltaTime)) {
        m_animator.applyTo(m_sprite);
    }

    // 2. Logique de comportement : le scientifique fait des allers-retours toutes les 2 secondes
    m_directionTimer += deltaTime;
    if (m_directionTimer >= 2.0f) {
        m_directionTimer = 0.f;

        // On inverse sa vitesse (s'il allait à gauche, il va à droite, et inversement)
        setSpeed(-getSpeed());

        // 3. Orientation du sprite selon le sens de déplacement
        if (getSpeed() > 0) {
            m_sprite.setScale({ 1.f, 1.f });   // Regarde à droite (vitesse positive)
        }
        else {
            m_sprite.setScale({ -1.f, 1.f });  // Regarde à gauche (vitesse négative)
        }
    }

    // 4. Application du déplacement physique sur l'axe X
    move(deltaTime);
}

void Scientist::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void Scientist::collide(Object&) {}
void Scientist::collide(Player&) {}