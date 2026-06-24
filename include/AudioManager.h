#pragma once
#include <SFML/Audio.hpp>
#include <list>
#include <string>

class AudioManager {
public:
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static AudioManager& getInstance();

    void playSound(const std::string& name);

    void playMusic(const std::string& filename, float volume = 35.f);
    void stopMusic();

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