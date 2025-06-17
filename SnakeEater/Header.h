#pragma once
//hvcv

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <thread>
#include <vector>
#include <ctime>
#include <random>
using namespace sf;
using namespace std;

#define SCREEN_RESX 1280
#define SCREEN_RESY 720
#define MAP_SIZEX 2500
#define MAP_SIZEY 2000
#define OBJECT_SIZE 30
#define PLAYER_SIZEX 30
#define PLAYER_SIZEY 50
#define MIN_NUM_OF_OBJECTS 200
#define MAX_NUM_OF_OBJECTS 500
#define STEP 5
#define SPEED 12   // < 10 - Faster; > 10 - Slower

extern int game_status;

class Object {
private:
    short int type;
    Vector2f pos;

public:
	Object(int type, float posX, float posY)
	{
		this->type = type;
		this->pos = {posX, posY};
	};
	int getType() {
		return type;
	}
	Vector2f getPos() {
		return pos;
	}
};

extern vector<Object> objects;
extern RectangleShape GrassShape;
extern RectangleShape RockShape;
extern RectangleShape StumpShape;
extern RectangleShape BushShape;
extern RectangleShape Player;

//FUNCTIONS

//Kutsenko Roman
void setObjectsPos();
void fillTheMapWithObj(RenderWindow* window);
