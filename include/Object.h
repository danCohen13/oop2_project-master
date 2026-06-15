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

    // Le Double Dispatch polymorphique de base
    virtual void collide(Object& other) = 0;

    // Déclarations uniques pour chaque classe concrète (VÉRIFIEZ BIEN CETTE ZONE)
    virtual void collide(Player&) {}
    virtual void collide(Coin&) {}     // UNE SEULE LIGNE POUR COIN !
    virtual void collide(Laser&) {}   // UNE SEULE LIGNE POUR LASER !

    virtual bool isDisposed() const { return false; }

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;

protected:
    sf::Sprite m_sprite;
};