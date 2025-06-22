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

sf::SoundBuffer buttonClickBuff;
std::unique_ptr<sf::Sound> buttonClickSound;

sf::SoundBuffer antidoteBuff;
std::unique_ptr<sf::Sound> antidoteSound;

sf::SoundBuffer snakeBiteBuff;
std::unique_ptr<sf::Sound> snakeBiteSound;

sf::SoundBuffer timerBuff;
std::unique_ptr<sf::Sound> timerSound;

sf::Music deathMusic;

sf::Music menuMusic;

bool MenuMusicFunc() {
    if (!menuMusic.openFromFile("backMusic file/menu_music.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл menu_music.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    menuMusic.setLooping(true);
    menuMusic.setPitch(0.9f);
    menuMusic.setVolume(10);
    return true;
}

void PlayMenuMusic() {
    backMusic.stop();
    deathMusic.stop();
    menuMusic.play();
}

void StopMenuMusic() {
    menuMusic.stop();
}

bool DeathMusicFunc() {
    if (!deathMusic.openFromFile("backMusic file/fail_music.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл fail_music.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    deathMusic.setLooping(false);
    deathMusic.setVolume(35);
    return true;
}

void PlayDeathMusic() {
    backMusic.setVolume(0);
    backMusic.stop();
    deathMusic.stop();
    deathMusic.play();
}

void StopDeathMusic() {
    deathMusic.stop();
}

bool TimerSoundFunc() {
    if (!timerBuff.loadFromFile("backMusic file/oclock_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл oclock_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    timerSound = std::make_unique<sf::Sound>(timerBuff);
    return true;
}

void PlayTimerSound(float speed) {
    if (timerSound) {
        timerSound->setVolume(70);
        timerSound->setPitch(speed);
        if (timerSound->getStatus() != sf::SoundSource::Status::Playing)
            timerSound->play();
    }
}

void StopTimerSound() {
    if (timerSound && timerSound->getStatus() == sf::SoundSource::Status::Playing) {
        timerSound->stop();
    }
}

bool SnakeBiteSoundFunc() {
    if (!snakeBiteBuff.loadFromFile("backMusic file/bite_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл bite_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    snakeBiteSound = std::make_unique<sf::Sound>(snakeBiteBuff);
    return true;
}

void PlaySnakeBiteSound() {
    if (snakeBiteSound && snakeBiteSound->getStatus() != sf::SoundSource::Status::Playing) {
        snakeBiteSound->setVolume(60);
        snakeBiteSound->play();
    }
}

bool AntidoteSoundFunc() {
    if (!antidoteBuff.loadFromFile("backMusic file/antidote_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл antidote_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    antidoteSound = std::make_unique<sf::Sound>(antidoteBuff);
    return true;
}

void PlayAntidoteSound() {
    if (antidoteSound && antidoteSound->getStatus() != sf::SoundSource::Status::Playing) {
        antidoteSound->setVolume(140);
        antidoteSound->play();
    }
}

bool ButtonClickSoundFunc() {
    if (!buttonClickBuff.loadFromFile("backMusic file/click_sound.ogg")) {
        MessageBoxA(NULL, "Не вдалося завантажити файл click_sound.ogg", "Помилка", MB_OK | MB_ICONERROR);
        return false;
    }
    buttonClickSound = std::make_unique<sf::Sound>(buttonClickBuff);
    return true;
}

void PlayButtonClickSound() {
    if (buttonClickSound && buttonClickSound->getStatus() != sf::SoundSource::Status::Playing) {
        buttonClickSound->setVolume(100);
        buttonClickSound->play();
    }
}

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

void lowerMusic(float tgVolume) {
    backMusic.setVolume(tgVolume);
}

void restoreMusic(float defaultVolume) {
    backMusic.setVolume(defaultVolume);
}