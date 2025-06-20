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
#define PLAYER_SIZEX 66
#define PLAYER_SIZEY 78
#define MIN_NUM_OF_OBJECTS 300
#define MAX_NUM_OF_OBJECTS 600
#define STEP 5
#define SPEED 12   // < 10 - Faster; > 10 - Slower
#define MIN_SNAKE_SIZE 4
#define MAX_SNAKE_SIZE 7
#define SNAKE_SPEED 400
#define SNAKE_MOVES 2
#define HIT_DELAY 800

extern float playerPosX;
extern float playerPosY;
extern int game_status;
extern int score;

class Object {
private:
	short int type;
	Vector2f pos;

public:
	Object(int type, float posX, float posY)
	{
		this->type = type;
		this->pos = { posX, posY };
	};
	int getType() {
		return type;
	}
	Vector2f getPos() {
		return pos;
	}
};

class SnakeBody {
public:
	short int bodyDirect;
	Vector2f pos;

	SnakeBody(int direct, Vector2f pos) {
		this->bodyDirect = direct;
		this->pos = pos;
	}

};

class Snake {
private:
	short int direction = 0;
	short int size;
	vector<SnakeBody> body;

public:
	Snake(int size, int direction, vector<SnakeBody>& body)
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
	vector<SnakeBody>& getBody() {
		return body;
	}
	void hitSnake() {
		this->size -= 1;
		this->body.pop_back();
	}
};

extern vector<Object> objects;
extern vector<Snake> snakes;

extern Sprite* GrassSprite;
extern Sprite* RockSprite;
extern Sprite* StumpSprite;
extern Sprite* BushSprite;
extern Sprite* PlayerSprite;
extern Sprite* SnakeBodySprite;
extern Sprite* SnakeHeadSprite;
extern Sprite* SnakeTailSprite;

extern Texture PlayerFrontTexture;
extern Texture RockTexture;
extern Texture BushTexture;
extern Texture SnakeBodyTexture;
extern Texture SnakeBodyBendTexture;
extern Texture SnakeHeadTexture;
extern Texture SnakeTailTexture;

//FUNCTIONS

//Kutsenko Roman
void setObjectsPos();
void fillTheMapWithObj(RenderWindow* window);
bool isPosNew(Vector2f pos);
void spawnSnakes(int count);
void moveSnakes();
void drawSnakes(RenderWindow* window);
void attackSnake(Vector2f playerPos, int playerDirect);
void deleteSnakes();

//VikaK
void handleZoom(View& view);

// Zadumina Zlata
void AudioTrack();
bool FootStepSound();
void PlayFootStepSound();