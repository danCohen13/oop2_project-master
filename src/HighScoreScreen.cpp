#include "HighScoreScreen.h"
#include "Resources.h"
#include <fstream>
#include <algorithm>

std::vector<ScoreEntry> HighScoreScreen::loadScores() {
    std::vector<ScoreEntry> scores;
    std::ifstream file("resources/highscores.txt");
    if (!file.is_open()) return scores;

    ScoreEntry entry;
    while (file >> entry.distance >> entry.coins) {
        scores.push_back(entry);
    }
    file.close();
    return scores;
}

void HighScoreScreen::saveScore(int distance, int coins) {
    auto scores = loadScores();
    scores.push_back({ distance, coins });

    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        if (a.distance != b.distance) return a.distance > b.distance;
        return a.coins > b.coins;
        });

    if (scores.size() > 5) {
        scores.resize(5);
    }

    std::ofstream file("resources/highscores.txt");
    if (!file.is_open()) return;

    for (const auto& s : scores) {
        file << s.distance << " " << s.coins << "\n";
    }
    file.close();
}

HighScoreScreen::HighScoreScreen(ScreenStack& stack)
    : Screen(stack),
    m_backgroundSprite(Resources::getInstance().getTexture("start_background")),
    m_titleText(Resources::getInstance().getFont()),
    m_mousePos(0.f, 0.f)
{
    auto& resources = Resources::getInstance();
    const auto& font = resources.getFont();

    auto bgSize = resources.getTexture("start_background").getSize();
    m_backgroundSprite.setScale({ 1200.f / bgSize.x, 600.f / bgSize.y });

    m_titleText.setString("TOP 5 HIGH SCORES");
    m_titleText.setCharacterSize(45);
    m_titleText.setFillColor(sf::Color(255, 215, 0)); 
    m_titleText.setStyle(sf::Text::Bold);
    auto titleBounds = m_titleText.getLocalBounds().size;
    m_titleText.setOrigin({ titleBounds.x / 2.f, titleBounds.y / 2.f });
    m_titleText.setPosition({ 600.f, 80.f });

    auto scores = loadScores();
    for (size_t i = 0; i < 5; ++i) {
        sf::Text rowText(font);
        rowText.setCharacterSize(26);
        rowText.setFillColor(sf::Color::White);

        if (i < scores.size()) {
            rowText.setString(std::to_string(i + 1) + ".  " +
                std::to_string(scores[i].distance) + " m   ---   " +
                std::to_string(scores[i].coins) + " Coins");
        }
        else {
            rowText.setString(std::to_string(i + 1) + ".  --- m   ---   --- Coins");
            rowText.setFillColor(sf::Color(150, 150, 150, 150)); 
        }

        auto rowBounds = rowText.getLocalBounds().size;
        rowText.setOrigin({ rowBounds.x / 2.f, rowBounds.y / 2.f });
        rowText.setPosition({ 600.f, 180.f + (static_cast<float>(i) * 55.f) });
        m_scoreTexts.push_back(rowText);
    }

    m_backButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 510.f }, sf::Vector2f{ 200.f, 50.f }, "Back", font);
}

void HighScoreScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) };
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x), static_cast<float>(mouseReleased->position.y) };

            if (m_backButton->isClicked(clickPos)) {
                m_stack.pop(); 
            }
        }
    }
}

void HighScoreScreen::update(float deltaTime) {
    m_backButton->update(m_mousePos, deltaTime);
}

void HighScoreScreen::draw(sf::RenderWindow& window) {
    window.draw(m_backgroundSprite);
    window.draw(m_titleText);

    for (const auto& text : m_scoreTexts) {
        window.draw(text);
    }

    m_backButton->draw(window);
}