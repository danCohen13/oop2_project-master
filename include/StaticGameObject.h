#pragma once
#include "Object.h"
#include "SpriteAnimator.h"

class StaticGameObject : public Object {
public:
    StaticGameObject(const sf::Texture& texture, const sf::Vector2f& position, int frameCount = 1);
    virtual ~StaticGameObject() override = default;

    virtual void update(float deltaTime) override;

protected:
    SpriteAnimator m_animator; 
};