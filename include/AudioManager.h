#pragma once
#include <SFML/Audio.hpp>
#include <list>
#include <string>

class AudioManager {
public:
    // Empêcher la copie du Singleton
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static AudioManager& getInstance();

    // Jouer un effet sonore court (gère sa propre fin de vie)
    void playSound(const std::string& name);

    // Gestion de la musique de fond continue
    void playMusic(const std::string& filename, float volume = 35.f);
    void stopMusic();

    // Fonctionnalités globales indispensables
    void setMuted(bool muted);
    bool isMuted() const;
    void cleanStoppedSounds();

private:
    AudioManager() = default;
    ~AudioManager() = default;

    std::list<sf::Sound> m_activeSounds;
    sf::Music m_music;
    bool m_isMuted = false;
};