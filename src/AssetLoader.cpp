#include "AssetLoader.h"
#include "Resources.h"
#include "AudioManager.h"

namespace AssetLoader {

    namespace {
        void loadTextures() {
            auto& resources = Resources::getInstance();

            resources.loadTexture("warehouse_background", "resources/warehouse_background.png");
            resources.loadTexture("start_background", "resources/GameMenu.png");
            resources.loadTexture("background", "resources/Hall.png");
            resources.loadTexture("player", "resources/JetpackWalking.png");
            resources.loadTexture("PlayerDead", "resources/PlayerDead.png");
            resources.loadTexture("Exhaust", "resources/Exhaust.png");
            resources.loadTexture("Coin", "resources/Coin.png");
            resources.loadTexture("Laser", "resources/Laser.png");
            resources.loadTexture("Missile", "resources/Missile.png");
            resources.loadTexture("MissileWarning", "resources/MissileWarning.png");
            resources.loadTexture("MissileIncoming", "resources/MissileIncoming.png");

            resources.loadTexture("play_btn", "resources/play.png");
            resources.loadTexture("rules_btn", "resources/rules.png");
            resources.loadTexture("button_menu", "resources/ButtonMenu.png");
            resources.loadTexture("Title", "resources/Title.png");
            resources.loadTexture("help1", "resources/help1.PNG");
            resources.loadTexture("help2", "resources/help2.PNG");
            resources.loadTexture("help3", "resources/help3.PNG");
            resources.loadTexture("back_btn", "resources/back.png");
            resources.loadTexture("next_btn", "resources/continue.png");
            resources.loadTexture("Scientist", "resources/Scientist.png");

            resources.loadTexture("Speed", "resources/SpeedGift.png");
            resources.loadTexture("speedflame", "resources/speedflame.png");
            resources.loadTexture("PowerUpBox", "resources/PowerUpBox.png");
            resources.loadTexture("SuperPowerRunner", "resources/SuperPowerRunner.png");
            resources.loadTexture("pause_btn", "resources/pause.png");
        }

        void loadSounds() {
            auto& resources = Resources::getInstance();

            resources.loadSound("missile_alarm", "resources/MissileAlarm.wav");
            resources.loadSound("piggy", "resources/Piggy.wav");
            resources.loadSound("shield_speed", "resources/ShieldSpeed.ogg");
            resources.loadSound("zapper", "resources/ZapperSound.wav");
            resources.loadSound("coin", "resources/coin.wav");
            resources.loadSound("get_power", "resources/getPowerSound.wav");
            resources.loadSound("hit", "resources/hitSound.wav");
            resources.loadSound("missile_launch", "resources/missileLaunch.wav");
        }

        void loadFonts() {
            Resources::getInstance().loadFont("resources/New Athletic M54.ttf");
        }

        void startMusic() {
            AudioManager::getInstance().playMusic("resources/JetpackJoyrideMusic.wav", 35.f);
        }
    }

    void loadAll() {
        loadTextures();
        loadSounds();
        loadFonts();
        startMusic();
    }

}