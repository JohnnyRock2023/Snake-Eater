#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <thread>
#include <vector>
#include <ctime>
#include <string>
#include <random>
#include <cmath>
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
#define MIN_SNAKE_SIZE 3
#define MAX_SNAKE_SIZE 7
#define SNAKE_SPEED 500
#define SNAKE_MOVES 2

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

class Snake {
private:
	short int direction = 0;
	short int size;
	vector<Vector2f> body;

public:
	Snake(int size, int direction, vector<Vector2f> &body)
	{
		this->size = size;
		this->body = body;
		this->direction = direction;
	};
	int getSize() {
		return size;
	}
	int getDirect() {
		return direction;
	}
	void setDirect(int direction) {
		this->direction = direction;
	}
	vector<Vector2f>& getBody() {
		return body;
	}
	void setBody(vector<Vector2f> body) {
		this->body = body;
	}
};

extern vector<Object> objects;
extern vector<Snake> snakes;
extern RectangleShape GrassShape;
extern RectangleShape RockShape;
extern RectangleShape StumpShape;
extern RectangleShape BushShape;
extern RectangleShape Player;
extern RectangleShape SnakeBodyShape;
extern RectangleShape SnakeHeadShape;

//FUNCTIONS

//Kutsenko Roman
void setObjectsPos();
void fillTheMapWithObj(RenderWindow* window);
bool isPosNew(Vector2f pos);
void spawnSnakes(int count);
void moveSnakes();
void drawSnakes(RenderWindow* window);
