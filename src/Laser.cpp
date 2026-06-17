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
    // Hitbox étroite et centrée (indépendante du sprite, tourne avec lui)
    m_hitbox.setSize({ 30.f, 350.f });
    m_hitbox.setOrigin({ 15.f, 175.f });
    m_hitbox.setPosition(position);

    // L'origine du sprite est déjà centrée par StaticGameObject,
    // mais on recalcule ici pour s'assurer que la rotation s'applique au bon point.
    auto textureSize = m_sprite.getTexture().getSize();
    float frameWidth = static_cast<float>(textureSize.x) / 4.f;
    float frameHeight = static_cast<float>(textureSize.y);
    m_sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    // Angle de départ aléatoire (multiple de 10°, entre 0 et 170°)
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
    // 1. Délègue la rotation à l'état courant (Static ou Rotating)
    m_currState->rotate(this, deltaTime);

    // 2. Animation de texture via SpriteAnimator (cadence propre au laser : 0.09s)
    //    On crée un animator local avec sa propre durée plutôt que la durée par défaut.
    //    NOTE : le timer de vibration est indépendant de l'animation de frame.
    m_animationTimer += deltaTime;
    if (m_animationTimer >= 0.09f) {
        m_animationTimer = 0.f;
        // On avance manuellement car le laser a une frameDuration différente (0.09s ≠ 0.1s)
        // et StaticGameObject::update() utiliserait 0.1s. On garde le contrôle ici.
        m_currentFrame = (m_currentFrame + 1) % 4;

        auto texSize = m_sprite.getTexture().getSize();
        int fw = texSize.x / 4;
        int fh = texSize.y;
        m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * fw, 0 }, { fw, fh }));
    }

    // 3. Vibration organique sur l'axe X (pulsation d'énergie)
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
        // Si le joueur a le pouvoir de gravité, le laser explose/disparaît à l'impact
        if (player.isSuperPowerRunner()) {
            m_isDisposed = true;
        }
        player.setDead(true); // Déclenche le retrait du pouvoir géré par Player::setDead
    }
}

bool Laser::isDisposed() const {
    return m_isDisposed;
}