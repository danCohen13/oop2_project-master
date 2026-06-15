#include "Exhaust.h"
#include "Resources.h"

Exhaust::Exhaust()
    : m_sprite(Resources::getInstance().getTexture("Exhaust")),
    m_animator(Resources::getInstance().getTexture("Exhaust"), 6) // 6 frames dans Exhaust.png
{
    // On applique la première frame dès la construction
    m_animator.applyTo(m_sprite);

    // Origine au centre du bord supérieur de la flamme
    auto size = m_animator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(size.x) / 2.f, 0.f });
}

void Exhaust::setActive(bool active) {
    if (!active && m_active) {
        // On remet l'animation à zéro à l'extinction pour repartir proprement
        m_animator.reset();
        m_animator.applyTo(m_sprite);
    }
    m_active = active;
}

void Exhaust::update(float deltaTime, const sf::Vector2f& playerPos) {
    if (!m_active) return;

    // Avance l'animation de façon autonome
    if (m_animator.update(deltaTime))
        m_animator.applyTo(m_sprite);

    // Positionne la flamme sous le jetpack
    m_sprite.setPosition({ playerPos.x + OFFSET_X, playerPos.y + OFFSET_Y });
}

void Exhaust::syncFrame(int playerFrame) {
    // Inutile d'animer si la flamme est éteinte
    if (!m_active) return;

    // Applique directement la frame demandée sans toucher au timer :
    // WalkState et JumpState peuvent ainsi forcer la synchronisation.
    // On utilise applyTo après avoir forcé la frame dans l'animator via reset + N ticks
    // → solution plus simple : on manipule le sprite directement ici.
    const sf::Texture& tex = Resources::getInstance().getTexture("Exhaust");
    int fw = tex.getSize().x / 6;
    int fh = tex.getSize().y;
    m_sprite.setTextureRect(sf::IntRect({ playerFrame * fw, 0 }, { fw, fh }));
}

void Exhaust::draw(sf::RenderWindow& window) const {
    if (m_active)
        window.draw(m_sprite);
}