#pragma once
#include "MovingGameObject.h" // Changement de parent
#include "SpriteAnimator.h"   // Ajout de l'animateur par composition

class Scientist : public MovingGameObject {
public:
    Scientist(const sf::Vector2f& position);
    virtual ~Scientist() override = default;

    // On surcharge update pour gérer le mouvement ET l'animation à chaque frame
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) const override;

    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

private:
    SpriteAnimator m_animator;
    float m_directionTimer = 0.f; // Timer pour faire changer le scientifique de sens
};