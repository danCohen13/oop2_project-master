#pragma once
#include "Object.h"
#include "SpriteAnimator.h"

// StaticGameObject est la base de tous les objets animés non-mobiles (Coin, Laser).
// Il délègue toute la logique spritesheet à SpriteAnimator par composition.
class StaticGameObject : public Object {
public:
    StaticGameObject(const sf::Texture& texture, const sf::Vector2f& position, int frameCount = 1);
    virtual ~StaticGameObject() override = default;

    virtual void update(float deltaTime) override;

protected:
    SpriteAnimator m_animator; // Unique responsable des frames
};