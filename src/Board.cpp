#include "Board.h"
#include "CollisionManager.h"
#include "LevelGenerator.h"
#include "ObjectCleaner.h"
#include "GameObjectFactory.h" 
#include "Coin.h"
#include "Laser.h" 

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
}

Board::~Board() = default;

void Board::play(float deltaTime, float gameSpeed, bool isThrusting) {
    // 1. Réinitialisation des événements pour cette frame
    m_coinsCollected = 0;
    bool wasAlive = !m_player->isDead();

    // 2. Mise à jour du joueur (Gestion interne du Jetpack Y)
    m_player->update(deltaTime, isThrusting);

    // NOUVEAU : On force le joueur à avancer sur l'axe X à la vitesse de la session.
    // Comme la caméra (dans GamePlayScreen) suit le joueur, c'est ça qui crée le défilement !
    sf::Vector2f pos = m_player->getPosition();
    m_player->setPosition({ pos.x + (gameSpeed * deltaTime), pos.y });

    // 3. Mise à jour des objets statiques (ex: l'animation vibrante des lasers)
    for (auto& obj : m_objects) {
        obj->update(deltaTime);
    }

    // 4. Gestion des collisions (Double Dispatch)
    m_collisionManager->handleCollisions(*m_player, m_objects);

    // 5. Lecture des conséquences de la physique
    if (wasAlive && m_player->isDead()) {
        m_hazardHit = true; // Le laser a tué le joueur
    }

    for (const auto& obj : m_objects) {
        // Un objet marqué 'disposed' après une collision (ex: Coin) est considéré comme ramassé
        if (obj->isDisposed()) {
            m_coinsCollected++;
        }
    }

    // 6. Génération procédurale (basée sur la nouvelle position X du joueur)
    m_levelGenerator->generate(deltaTime, m_objects, *m_player);
    // 7. Nettoyage
    // L'ObjectCleaner va détruire les pièces ramassées et les objets laissés derrière la caméra
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