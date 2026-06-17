#include "SuperPowerRunnerState.h"
#include "Player.h"
#include "Resources.h"

SuperPowerRunnerState::SuperPowerRunnerState(Player& player)
    : m_animator(Resources::getInstance().getTexture("SuperPowerRunner"), 4, 0.06f), // 4 frames réelles
    m_gravityReversed(false),
    m_spaceWasPressed(false),
    m_verticalVelocity(0.f)
{
    auto& sprite = player.getSprite();
    sprite.setTexture(Resources::getInstance().getTexture("SuperPowerRunner"));
    m_animator.applyTo(sprite);

    // Origine centrée-basse : Le flip se fera parfaitement au niveau des pieds !
    auto size = m_animator.getFrameSize();
    sprite.setOrigin({ static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) });
    sprite.setScale({ 1.f, 1.f });
}

void SuperPowerRunnerState::update(Player& player, float deltaTime) {
    // 1. Animation des frames de course
    if (m_animator.update(deltaTime)) {
        m_animator.applyTo(player.getSprite());
    }

    // 2. Gestion de l'inversion de gravité (Appui unique sur Espace)
    bool spacePressed = player.isThrusting();
    if (spacePressed && !m_spaceWasPressed) {
        m_gravityReversed = !m_gravityReversed;

        // Retourne le personnage verticalement
        if (m_gravityReversed) {
            player.getSprite().setScale({ 1.f, -1.f }); // Tête en bas
        }
        else {
            player.getSprite().setScale({ 1.f, 1.f });  // Tête en haut
        }
    }
    m_spaceWasPressed = spacePressed;

    // 3. Calculs physiques de la fausse gravité
    float gravityDirection = m_gravityReversed ? -1.f : 1.f;
    m_verticalVelocity += gravityDirection * GRAVITY_ACCEL * deltaTime;

    sf::Vector2f pos = player.getSprite().getPosition();
    pos.y += m_verticalVelocity * deltaTime;

    // 4. Collisions avec le Sol et le Plafond
    const sf::Texture& normalTex = Resources::getInstance().getTexture("player");

    // CORRECTION : Comme l'origine est aux pieds du runner, la position Y au sol 
    // doit être égale à floorY + la hauteur réelle de la texture du joueur.
    float realFloorY = player.getFloorY() + static_cast<float>(normalTex.getSize().y);
    float realCeilingY = player.getCeilingY();

    if (!m_gravityReversed) {
        if (pos.y >= realFloorY) {
            pos.y = realFloorY; // Pieds calés parfaitement sur le sol
            m_verticalVelocity = 0.f;
        }
    }
    else {
        if (pos.y <= realCeilingY) {
            pos.y = realCeilingY; // Pieds calés parfaitement sur le plafond
            m_verticalVelocity = 0.f;
        }
    }

    player.getSprite().setPosition(pos);
}

void SuperPowerRunnerState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    // On dessine uniquement le joueur (pas de flamme jetpack sous les pieds en courant)
    window.draw(playerSprite);
}