#include "JumpState.h"
#include "Player.h"
#include "Resources.h"
#include "Exhaust.h"

JumpState::JumpState() : m_currentFrame(0), m_frameTimer(0.0f) {}

void JumpState::update(Player& player, float deltaTime) {
    // 1. Découpage du joueur sur sa frame en l'air (Dernière frame)
    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = playerTex.getSize().x / 4;
    int playerHeight = playerTex.getSize().y;
    player.getSprite().setTextureRect(sf::IntRect({ 3 * playerWidth, 0 }, { playerWidth, playerHeight }));

    // 2. On délègue TOUT le travail à l'objet Exhaust encapsulé dans Player
    player.getExhaust().setActive(player.isThrusting());
    player.getExhaust().update(deltaTime, player.getPosition());
}

void JumpState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    exhaust.draw(window);      // Dessine la flamme en premier (arrière-plan)
    window.draw(playerSprite); // Dessine le joueur par-dessus
}