#pragma once
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

struct ScoreEntry {
    int distance = 0;
    int coins = 0;
};

class HighScoreScreen : public Screen {
public:
    HighScoreScreen(ScreenStack& stack);
    virtual ~HighScoreScreen() override = default;

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

    static std::vector<ScoreEntry> loadScores();
    static void saveScore(int distance, int coins);

private:
    sf::Sprite m_backgroundSprite;
    sf::Text m_titleText;
    std::vector<sf::Text> m_scoreTexts;
    std::unique_ptr<Button> m_backButton;
    sf::Vector2f m_mousePos;
};