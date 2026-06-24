#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Coin;
class Laser;

class Object {
public:
    Object(const sf::Texture& texture);
    virtual ~Object() = default;

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void update(float deltaTime) = 0;

    virtual void collide(Object& other) = 0;

    virtual void collide(Player&) {}
    virtual void collide(Coin&) {}     
    virtual void collide(Laser&) {}   

    virtual bool isDisposed() const { return false; }

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    virtual sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;

protected:
    sf::Sprite m_sprite;
};