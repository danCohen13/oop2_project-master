#include "DeadState.h"
#include "Player.h"
#include "Resources.h"

DeadState::DeadState()
    : m_animator(
        Resources::getInstance().getTexture("PlayerDead"),
        // Déduction automatique du nombre de frames (Largeur / Hauteur)
        static_cast<int>(Resources::getInstance().getTexture("PlayerDead").getSize().x /
            Resources::getInstance().getTexture("PlayerDead").getSize().y),
        0.07f // Vitesse de la roulade au sol
    )
{
}

void DeadState::update(Player& player, float deltaTime) {
    // 1. Force l'application de la texture de mort (PlayerDead.png)
    player.getSprite().setTexture(Resources::getInstance().getTexture("PlayerDead"));

    // 2. Met à jour le chronomètre de l'animateur par composition
    m_animator.update(deltaTime);

    // 3. Écrase le rectangle de texture hérité des états de vie (course/saut)
    m_animator.applyTo(player.getSprite());

    // 4. Désactive les flammes du jetpack
    player.getExhaust().setActive(false);
}

void DeadState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    (void)exhaust; // On ignore l'exhaust car le jetpack est éteint à la mort
    window.draw(playerSprite);
}