#include "BackgroundSystem.h"
#include "Resources.h"
#include <cmath>

BackgroundSystem::BackgroundSystem(float screenWidth, float screenHeight)
    : m_screenWidth(screenWidth),
    m_screenHeight(screenHeight),
    m_startBackground(Resources::getInstance().getTexture("start_background"))
{
    // 1. Configuration de la pièce de départ unique (GameMenu)
    const sf::Texture& startTex = Resources::getInstance().getTexture("start_background");
    float startScale = m_screenHeight / static_cast<float>(startTex.getSize().y);
    m_startBackground.setScale({ startScale, startScale });
    m_startBackground.setPosition({ 0.0f, 0.0f });

    // 2. Calcul du nombre de tuiles nécessaires pour remplir l'écran de jeu
    const sf::Texture& bgTex = Resources::getInstance().getTexture("background");
    float scaleFactor = m_screenHeight / static_cast<float>(bgTex.getSize().y);
    float bgWidth = static_cast<float>(bgTex.getSize().x) * scaleFactor;

    int tilesNeeded = static_cast<int>(std::ceil(m_screenWidth / bgWidth)) + 2;
    if (tilesNeeded < 3) tilesNeeded = 3;

    for (int i = 0; i < tilesNeeded; ++i) {
        m_backgrounds.emplace_back(bgTex);
    }
}

void BackgroundSystem::update(float deltaTime, float playerX) {
    (void)deltaTime;

    const sf::Texture& startTex = Resources::getInstance().getTexture("start_background");
    float startScale = m_screenHeight / static_cast<float>(startTex.getSize().y);
    float startBgWidth = static_cast<float>(startTex.getSize().x) * startScale;

    // Récupération et Rognage (Crop) de Hall.png
    const sf::Texture& hallTex = Resources::getInstance().getTexture("background");
    int hallW = hallTex.getSize().x;
    int hallH = hallTex.getSize().y;
    sf::IntRect hallRect({ 2, 0 }, { hallW - 4, hallH });

    // Récupération et Rognage (Crop) de warehouse_background.png
    const sf::Texture& warehouseTex = Resources::getInstance().getTexture("warehouse_background");
    int wareW = warehouseTex.getSize().x;
    int wareH = warehouseTex.getSize().y;
    sf::IntRect warehouseRect({ 2, 0 }, { wareW - 4, wareH });

    float hallScaleY = m_screenHeight / static_cast<float>(hallH);
    float warehouseScaleY = m_screenHeight / static_cast<float>(wareH);
    float bgWidth = static_cast<float>(hallW) * hallScaleY;

    // Calcul de compensation d'échelle X suite au rognage anti-ligne noire
    float overlap = 1.0f;
    float hallScaleX = (bgWidth + overlap) / static_cast<float>(hallRect.size.x);
    float warehouseScaleX = (bgWidth + overlap) / static_cast<float>(warehouseRect.size.x);

    float viewLeft = playerX - 200.0f;

    int baseTileIndex = static_cast<int>(std::floor((viewLeft - startBgWidth) / bgWidth));
    if (baseTileIndex < 0) baseTileIndex = 0;

    float startX = static_cast<float>(baseTileIndex) * bgWidth + startBgWidth;
    if (startX < startBgWidth) startX = startBgWidth;

    // ALTERNANCE : 10 Hall / 6 Warehouse (Total de la période = 16)
    for (size_t i = 0; i < m_backgrounds.size(); ++i) {
        int currentTileIndex = baseTileIndex + static_cast<int>(i);
        float currentX = startX + static_cast<float>(i) * bgWidth;

        m_backgrounds[i].setPosition({ currentX, 0.0f });

        int remainder = currentTileIndex % 16;

        if (remainder < 10) {
            m_backgrounds[i].setTexture(hallTex);
            m_backgrounds[i].setTextureRect(hallRect);
            m_backgrounds[i].setScale({ hallScaleX, hallScaleY });
        }
        else {
            m_backgrounds[i].setTexture(warehouseTex);
            m_backgrounds[i].setTextureRect(warehouseRect);
            m_backgrounds[i].setScale({ warehouseScaleX, warehouseScaleY });
        }
    }
}

void BackgroundSystem::draw(sf::RenderWindow& window) const {
    window.draw(m_startBackground);
    for (const auto& bg : m_backgrounds) {
        window.draw(bg);
    }
}