#include "Header.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <filesystem>

using namespace sf;
using namespace std;

sf::Music backMusic;

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