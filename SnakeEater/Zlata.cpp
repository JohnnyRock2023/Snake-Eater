#include "Header.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <filesystem>

using namespace sf;
using namespace std;

sf::Music backMusic;

sf::SoundBuffer stepBuff;
std::unique_ptr<sf::Sound> stepSound;

sf::SoundBuffer hitSnakeBuff;
std::unique_ptr<sf::Sound> hitSnakeSound;

bool HitSnakeSound() {
    if (!hitSnakeBuff.loadFromFile("backMusic file/eat_snake_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл eat_snake_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    hitSnakeSound = std::make_unique<sf::Sound>(hitSnakeBuff);
    return true;
}

void PlayHitSnakeSound() {
    if (hitSnakeSound && hitSnakeSound->getStatus() != sf::SoundSource::Status::Playing) {
        hitSnakeSound->setVolume(150);
        hitSnakeSound->play();
    }
}

bool FootStepSound() {
    if (!stepBuff.loadFromFile("backMusic file/footstep_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл foot_step_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    stepSound = std::make_unique<sf::Sound>(stepBuff);
    return true;
}

void PlayFootStepSound() {
    if (stepSound && stepSound->getStatus() != sf::SoundSource::Status::Playing) {
        stepSound->setVolume(30);
        stepSound->play();
    }
}

void AudioTrack() {
    if (!backMusic.openFromFile("backMusic file/music_for_project.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл music_for_project.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return;
    }

    backMusic.setLooping(true);
    backMusic.setVolume(45);
    backMusic.play();
}

void updateMusicLoop() {
    if (backMusic.getStatus() == sf::SoundSource::Status::Stopped) {
        backMusic.play();
    }
}