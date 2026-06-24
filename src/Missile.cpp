#include "Missile.h"
#include "Player.h"
#include "Resources.h"
#include <cmath>

int determineFrameCount(const sf::Texture& texture) {
    int count = static_cast<int>(texture.getSize().x / texture.getSize().y);
    return (count > 0) ? count : 1;
}

Missile::Missile(Player& player, float yOffset)
    : MovingGameObject(Resources::getInstance().getTexture("Missile"), 650.0f),
    m_player(player),
    m_status(MissileStatus::Warning),
    m_yOffset(yOffset), 
    m_missileAnimator(Resources::getInstance().getTexture("Missile"), 7, 0.05f),
    m_warningAnimator(Resources::getInstance().getTexture("MissileWarning"), determineFrameCount(Resources::getInstance().getTexture("MissileWarning")), 0.08f),
    m_incomingAnimator(Resources::getInstance().getTexture("MissileIncoming"), determineFrameCount(Resources::getInstance().getTexture("MissileIncoming")), 0.05f),
    m_warningSprite(Resources::getInstance().getTexture("MissileWarning")),
    m_warningTimer(0.0f),
    m_isDisposed(false)
{
    m_missileAnimator.applyTo(m_sprite);
    auto missileSize = m_missileAnimator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(missileSize.x) / 2.0f, static_cast<float>(missileSize.y) / 2.0f });

    m_warningAnimator.applyTo(m_warningSprite);
    auto warningSize = m_warningAnimator.getFrameSize();
    m_warningSprite.setOrigin({ static_cast<float>(warningSize.x) / 2.0f, static_cast<float>(warningSize.y) / 2.0f });

    m_sprite.setPosition({ player.getPosition().x + 2000.0f, 0.0f });
}

void Missile::update(float deltaTime) {
    float cameraX = m_player.getPosition().x + (VIRTUAL_SCREEN_WIDTH / 3.0f);
    if (cameraX < VIRTUAL_SCREEN_WIDTH / 2.0f) {
        cameraX = VIRTUAL_SCREEN_WIDTH / 2.0f;
    }

    if (m_status == MissileStatus::Warning) {
        m_warningTimer += deltaTime;

        float targetY = m_player.getPosition().y + 40.0f + m_yOffset;
        if (targetY < 60.0f)  targetY = 60.0f;
        if (targetY > 520.0f) targetY = 520.0f;

        float warningX = cameraX + (VIRTUAL_SCREEN_WIDTH / 2.0f) - 45.0f;

        m_warningSprite.setPosition({ warningX, targetY });
        m_sprite.setPosition({ warningX, targetY });

        if (m_warningTimer > WARNING_DURATION * 0.7f) {
            m_incomingAnimator.update(deltaTime);
            m_incomingAnimator.applyTo(m_warningSprite);
            m_warningSprite.setTexture(Resources::getInstance().getTexture("MissileIncoming"));

            auto size = m_incomingAnimator.getFrameSize();
            m_warningSprite.setOrigin({ static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f });
        }
        else {
            m_warningAnimator.update(deltaTime);
            m_warningAnimator.applyTo(m_warningSprite);
            m_warningSprite.setTexture(Resources::getInstance().getTexture("MissileWarning"));

            auto size = m_warningAnimator.getFrameSize();
            m_warningSprite.setOrigin({ static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f });
        }

        if (m_warningTimer >= WARNING_DURATION) {
            m_status = MissileStatus::Flying;
            m_sprite.setPosition({ cameraX + (VIRTUAL_SCREEN_WIDTH / 2.0f) + 80.0f, targetY });
        }
    }
    else if (m_status == MissileStatus::Flying) {
        m_sprite.move({ -m_speed * deltaTime, 0.0f });

        if (m_missileAnimator.update(deltaTime)) {
            m_missileAnimator.applyTo(m_sprite);
        }
    }
}

void Missile::draw(sf::RenderWindow& window) const {
    if (m_status == MissileStatus::Warning) {
        window.draw(m_warningSprite);
    }
    else if (m_status == MissileStatus::Flying) {
        window.draw(m_sprite);
    }
}

sf::FloatRect Missile::getGlobalBounds() const {
    if (m_status == MissileStatus::Warning) {
        return sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f });
    }

    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    bounds.size.x *= 0.40f;

    float originalHeight = bounds.size.y;
    bounds.size.y *= 0.80f;
    bounds.position.y += (originalHeight - bounds.size.y) / 2.0f; 

    return bounds;
}

void Missile::collide(Object& other) {
    other.collide(*this);
}

void Missile::collide(Player& player) {
    if (player.isSpeedBoosting()) {
        m_isDisposed = true; 
    }
    else if (player.isSuperPowerRunner()) {
        m_isDisposed = true;
        player.setDead(true);
    }
    else {
        player.setDead(true); 
    }
}