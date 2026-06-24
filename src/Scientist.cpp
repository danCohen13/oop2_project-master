#include "Scientist.h"
#include "Resources.h"

Scientist::Scientist(const sf::Vector2f& position)
    : MovingGameObject(Resources::getInstance().getTexture("Scientist"), -60.f), 
    m_animator(Resources::getInstance().getTexture("Scientist"), 3)             
{
    m_animator.applyTo(m_sprite);

    sf::Vector2i frameSize = m_animator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(frameSize.x) / 2.0f, static_cast<float>(frameSize.y) });
    m_sprite.setPosition(position);

    m_sprite.setScale({ -1.f, 1.f });
}

void Scientist::update(float deltaTime) {
    if (m_animator.update(deltaTime)) {
        m_animator.applyTo(m_sprite);
    }

    m_directionTimer += deltaTime;
    if (m_directionTimer >= 2.0f) {
        m_directionTimer = 0.f;

        setSpeed(-getSpeed());

        if (getSpeed() > 0) {
            m_sprite.setScale({ 1.f, 1.f });   
        }
        else {
            m_sprite.setScale({ -1.f, 1.f });  
        }
    }

    move(deltaTime);
}

void Scientist::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

void Scientist::collide(Object&) {}
void Scientist::collide(Player&) {}