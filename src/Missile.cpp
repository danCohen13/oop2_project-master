#include "Missile.h"
#include "Player.h"
#include "Resources.h"
#include <cmath>

Missile::Missile(Player& player)
    : MovingGameObject(Resources::getInstance().getTexture("Missile"), 650.0f),
    m_player(player),
    m_status(MissileStatus::Warning),
    m_animator(Resources::getInstance().getTexture("Missile"), 4, 0.05f),
    m_warningSprite(Resources::getInstance().getTexture("MissileWarning")),
    m_warningTimer(0.0f),
    m_isDisposed(false)
{
    m_animator.applyTo(m_sprite);
    auto missileSize = m_animator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(missileSize.x) / 2.0f, static_cast<float>(missileSize.y) / 2.0f });

    auto warningTexSize = Resources::getInstance().getTexture("MissileWarning").getSize();
    m_warningSprite.setOrigin({ static_cast<float>(warningTexSize.x) / 2.0f, static_cast<float>(warningTexSize.y) / 2.0f });
}

void Missile::update(float deltaTime) {
    float cameraX = m_player.getPosition().x + (VIRTUAL_SCREEN_WIDTH / 3.0f);
    if (cameraX < VIRTUAL_SCREEN_WIDTH / 2.0f) {
        cameraX = VIRTUAL_SCREEN_WIDTH / 2.0f;
    }

    if (m_status == MissileStatus::Warning) {
        m_warningTimer += deltaTime;

        float targetY = m_player.getPosition().y + 40.0f;
        float warningX = cameraX + (VIRTUAL_SCREEN_WIDTH / 2.0f) - 45.0f;
        m_warningSprite.setPosition({ warningX, targetY });

        // CORRECTION / FIX ANTI-CLEANUP : On déplace aussi m_sprite ici !
        // Comme warningX est devant le joueur, l'ObjectCleaner ne supprimera plus le missile.
        m_sprite.setPosition({ warningX, targetY });

        if (m_warningTimer > WARNING_DURATION * 0.7f) {
            m_warningSprite.setTexture(Resources::getInstance().getTexture("MissileIncoming"));
        }

        if (m_warningTimer >= WARNING_DURATION) {
            m_status = MissileStatus::Flying;
            m_sprite.setPosition({ cameraX + (VIRTUAL_SCREEN_WIDTH / 2.0f) + 80.0f, targetY });
        }
    }
    else if (m_status == MissileStatus::Flying) {
        m_sprite.move({ -m_speed * deltaTime, 0.0f });

        if (m_animator.update(deltaTime)) {
            m_animator.applyTo(m_sprite);
        }
    }
}

void Missile::draw(sf::RenderWindow& window) const {
    if (m_status == MissileStatus::Warning) {
        if (static_cast<int>(m_warningTimer / 0.15f) % 2 == 0) {
            window.draw(m_warningSprite);
        }
    }
    else if (m_status == MissileStatus::Flying) {
        window.draw(m_sprite);
    }
}

void Missile::collide(Object& other) {
    other.collide(*this);
}

void Missile::collide(Player& player) {
    if (m_status == MissileStatus::Flying && !player.isDead()) {
        player.setDead(true);
        m_isDisposed = true;
    }
}