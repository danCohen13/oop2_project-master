#include "Laser.h"
#include "Resources.h"
#include "Player.h"
#include "LaserStaticState.h"
#include "LaserRotatingState.h"
#include <cstdlib>
#include <cmath>

Laser::Laser(const sf::Vector2f& position)
    : StaticGameObject(Resources::getInstance().getTexture("Laser"), position, 4)
{
    m_hitbox.setSize({ 30.f, 350.f });
    m_hitbox.setOrigin({ 15.f, 175.f });
    m_hitbox.setPosition(position);

    auto textureSize = m_sprite.getTexture().getSize();
    float frameWidth = static_cast<float>(textureSize.x) / 4.f;
    float frameHeight = static_cast<float>(textureSize.y);
    m_sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    float angle = static_cast<float>((rand() % 18) * 10);
    rotate(angle);

    if (rand() % 2 == 0)
        m_currState = std::make_unique<LaserStaticState>();
    else
        m_currState = std::make_unique<LaserRotatingState>();
}

void Laser::rotate(float angle) {
    m_sprite.rotate(sf::degrees(angle));
    m_hitbox.rotate(sf::degrees(angle));
}

void Laser::update(float deltaTime) {
    m_currState->rotate(this, deltaTime);

    m_animationTimer += deltaTime;
    if (m_animationTimer >= 0.09f) {
        m_animationTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % 4;

        auto texSize = m_sprite.getTexture().getSize();
        int fw = texSize.x / 4;
        int fh = texSize.y;
        m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * fw, 0 }, { fw, fh }));
    }

    m_vibrationTimer += deltaTime;
    float wave = std::sin(m_vibrationTimer * 12.f);
    float targetScaleX = 1.f + (wave * 0.08f);
    float subtleNoise = 0.99f + static_cast<float>(rand() % 3) / 100.f;
    m_sprite.setScale({ targetScaleX * subtleNoise, 1.f });
}

void Laser::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void Laser::collide(Object& other) {
    other.collide(*this);
}

void Laser::collide(Player& player) {
    sf::FloatRect playerBounds = player.getGlobalBounds();

    sf::Vector2f pointsToTest[5] = {
        playerBounds.position,
        { playerBounds.position.x + playerBounds.size.x, playerBounds.position.y },
        { playerBounds.position.x, playerBounds.position.y + playerBounds.size.y },
        playerBounds.position + playerBounds.size,
        playerBounds.position + playerBounds.size / 2.f
    };

    sf::Transform  inverseTransform = m_hitbox.getInverseTransform();
    sf::FloatRect  localBounds = m_hitbox.getLocalBounds();

    bool realCollision = false;
    for (const auto& point : pointsToTest) {
        sf::Vector2f localPoint = inverseTransform.transformPoint(point);
        if (localBounds.contains(localPoint)) {
            realCollision = true;
            break;
        }
    }

    if (realCollision) {
        if (player.isSuperPowerRunner()) {
            m_isDisposed = true;
        }
        player.setDead(true); 
    }
}

bool Laser::isDisposed() const {
    return m_isDisposed;
}