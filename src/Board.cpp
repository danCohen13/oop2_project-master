#include "Board.h"
#include "CollisionManager.h"
#include "LevelGenerator.h"
#include "ObjectCleaner.h"
#include "GameObjectFactory.h" 
#include "Coin.h"
#include "Laser.h" 
#include "SpeedItem.h"
#include "Scientist.h"
#include "PowerUpBox.h"



Board::Board()
    : m_player(std::make_unique<Player>()),
    m_collisionManager(std::make_unique<CollisionManager>()),
    m_levelGenerator(std::make_unique<LevelGenerator>()),
    m_objectCleaner(std::make_unique<ObjectCleaner>()),
    m_coinsCollected(0),
    m_hazardHit(false)
{
    GameObjectFactory::registerType<Coin>("Coin");
    GameObjectFactory::registerType<Laser>("Laser");
    GameObjectFactory::registerType<SpeedItem>("SpeedItem");
    GameObjectFactory::registerType<Scientist>("Scientist"); 
    GameObjectFactory::registerType<PowerUpBox>("PowerUpBox");
}

Board::~Board() = default;

void Board::play(float deltaTime, float gameSpeed, bool isThrusting) {
    m_coinsCollected = 0;
    bool wasAlive = !m_player->isDead();

    m_player->update(deltaTime, isThrusting);

    sf::Vector2f pos = m_player->getPosition();
    m_player->setPosition({ pos.x + (gameSpeed * deltaTime), pos.y });

    for (auto& obj : m_objects) {
        obj->update(deltaTime);
    }

    m_collisionManager->handleCollisions(*m_player, m_objects);

    if (wasAlive && m_player->isDead()) {
        m_hazardHit = true; 
    }

    for (const auto& obj : m_objects) {
        if (obj->isDisposed()) {
            m_coinsCollected++;
        }
    }

    m_levelGenerator->generate(deltaTime, m_objects, *m_player);
    m_objectCleaner->cleanup(m_objects, m_player->getPosition().x);
}

void Board::draw(sf::RenderWindow& window) const {
    for (const auto& obj : m_objects) {
        obj->draw(window);
    }
    m_player->draw(window);
}

bool Board::isPlayerAlive() const { return !m_player->isDead(); }

sf::Vector2f Board::getPlayerPosition() const { return m_player->getPosition(); }

bool Board::hasPlayerHitHazard() const { return m_hazardHit; }

int Board::getCoinsCollectedThisFrame() const { return m_coinsCollected; }