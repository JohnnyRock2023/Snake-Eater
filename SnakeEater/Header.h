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

extern float SCREEN_RESX;
extern float SCREEN_RESY;
#define MAP_SIZEX 2500
#define MAP_SIZEY 2000
#define OBJECT_SIZE 30
#define PLAYER_SIZEX 66
#define PLAYER_SIZEY 78
#define MIN_NUM_OF_OBJECTS 200
#define MAX_NUM_OF_OBJECTS 400
#define STEP 5
#define SPEED 12   // < 10 - Faster; > 10 - Slower
#define MIN_SNAKE_SIZE 4
#define MAX_SNAKE_SIZE 7
#define SNAKE_SPEED 400
#define SNAKE_MOVES 2
#define NUM_OF_SNAKES 30
#define NUM_OF_ADDITIONAL_SNAKES 10
#define HIT_DELAY 500
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 90
#define DEATH 10
#define MIN_NUM_OF_ANTIDOTES 2
#define MAX_NUM_OF_ANTIDOTES 4
#define NUM_OF_ADDITIONAL_ANTIDOTES 2

extern float playerPosX;
extern float playerPosY;
extern float viewPosX;
extern float viewPosY;
extern int game_status;
extern short int playerDirection;
extern int score;
extern int bestScore;
extern bool isPoisoned;
extern float poisonTimer;
extern float pauseTimer;

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
	~Snake() {
		while (!body.empty()) {
			body.pop_back();
		}
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
		if (!body.empty()) {
			body.pop_back();
			size = (short)body.size();
		}
	}
};

extern vector<Object> objects;
extern vector<Snake> snakes;
extern vector<Vector2f> antidotes;

extern Sprite* GrassSprite;
extern Sprite* RockSprite;
extern Sprite* StumpSprite;
extern Sprite* BushSprite;
extern Sprite* PlayerSprite;
extern Sprite* SnakeBodySprite;
extern Sprite* SnakeHeadSprite;
extern Sprite* SnakeTailSprite;
extern Sprite* GroundSprite;
extern Sprite* StartButtonSprite;
extern Sprite* ExitButtonSprite;
extern Sprite* ContinueButtonSprite;
extern Sprite* RestartButtonSprite;
extern Sprite* MenuButtonSprite;
extern Sprite* LogoSprite;
extern Sprite* GameOverSprite;
extern Sprite* AntidoteSprite;
extern Sprite* ScullSprite;

extern Texture PlayerFrontTexture;
extern Texture RockLTexture;
extern Texture RockSTexture;
extern Texture BushLTexture;
extern Texture BushSTexture;
extern Texture SnakeBodyTexture;
extern Texture SnakeBodyBendTexture;
extern Texture SnakeHeadTexture;
extern Texture SnakeTailTexture;
extern Texture GroundTexture;
extern Texture StartButtonTexture;
extern Texture ExitButtonTexture;
extern Texture ContinueButtonTexture;
extern Texture RestartButtonTexture;
extern Texture MenuButtonTexture;
extern Texture LogoTexture;
extern Texture GameOverTexture;
extern Texture ScullTexture;
extern Texture AntidoteTexture;

extern Text* textScore;
extern Text* textBestScore;
extern Text* timeToDeath;

extern Clock* poisonClock;

extern View view;

//FUNCTIONS

//Kutsenko Roman
void setObjectsPos();
void fillTheMapWithObj(RenderWindow* window);
void spawnSnakes(int count);
void moveSnakes();
void drawSnakes(RenderWindow* window);
void attackSnake(int playerDirect);
void deleteSnakes();
bool inRange(int value, int a, int b);
void drawGround(RenderWindow* window);
bool isObjectPosNew(float posX, float posY);
bool isSnakePosNew(Vector2f pos);
void snakeBite();
void drawAntidotes(RenderWindow* window);
void setAntidotesPos(int count);
void useAntidote();
void displayScore(RenderWindow* window);
void displayTimeToDeath(RenderWindow* window);
void restart();
void isTheBest();
bool isNearPlayer(float posX, float posY);

//VikaK
void handleZoom(View& view);

// Zadumina Zlata
void AudioTrack();
bool FootStepSound();
void PlayFootStepSound();
bool HitSnakeSound();
void PlayHitSnakeSound();
bool ButtonClickSoundFunc();
void PlayButtonClickSound();
bool AntidoteSoundFunc();
void PlayAntidoteSound();
bool SnakeBiteSoundFunc();
void PlaySnakeBiteSound();
bool TimerSoundFunc();
void PlayTimerSound(float speed = 1.0f);
void StopTimerSound();

//Vika Pavlenko
void showStartMenu(RenderWindow* window);
void showPauseMenu(RenderWindow* window);
void showDeathScreen(RenderWindow* window);