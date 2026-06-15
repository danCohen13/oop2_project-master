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
    player.getSprite().setTexture(Resources::getInstance().getTexture("PlayerDead"));

    if (!m_finished) {
        bool frameChanged = m_animator.update(deltaTime);
        m_animator.applyTo(player.getSprite());

        if (frameChanged) {
            m_started = true; // au moins un changement de frame a eu lieu
        }

        // On détecte le wrap-around SEULEMENT après avoir démarré
        if (m_started && m_animator.getCurrentFrame() == 0) {
            m_finished = true;
            // Forcer manuellement la DERNIÈRE frame (frame 4 sur 5)
            const sf::Texture& tex = Resources::getInstance().getTexture("PlayerDead");
            int totalFrames = static_cast<int>(tex.getSize().x / tex.getSize().y);
            int fw = tex.getSize().x / totalFrames;
            int fh = tex.getSize().y;
            player.getSprite().setTextureRect(
                sf::IntRect({ (totalFrames - 1) * fw, 0 }, { fw, fh })
            );
        }
    }

    player.getExhaust().setActive(false);
}

void DeadState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    (void)exhaust; // On ignore l'exhaust car le jetpack est éteint à la mort
    window.draw(playerSprite);
}