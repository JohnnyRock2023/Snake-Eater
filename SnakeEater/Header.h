#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <thread>
#include <vector>
#include <ctime>
#include <string>
#include <random>
#include <cmath>
#include <mutex>
#include <queue>
#include <map>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

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
#define MAX_SNAKE_SIZE 9
#define SNAKE_SPEED 25
#define SNAKE_STEP 1
#define SNAKE_MOVES 2
#define NUM_OF_SNAKES 30
#define NUM_OF_ADDITIONAL_SNAKES 10
#define HIT_DELAY 500
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 90
#define DEATH 15
#define MIN_NUM_OF_ANTIDOTES 4
#define MAX_NUM_OF_ANTIDOTES 6
#define NUM_OF_ADDITIONAL_ANTIDOTES 2

extern float playerPosX;
extern float playerPosY;
extern float viewPosX;
extern float viewPosY;
extern int game_status;
extern short int playerDirection;
extern int coop_mode;
extern int score;
extern int bestScore;
extern bool isPoisoned;
extern float poisonTimer;
extern float pauseTimer;
extern int playerID;
extern int inviteCode;
extern int clientSock;
extern Font font;

extern mutex mtx1;
extern mutex mtx2;
extern mutex mtx_game_status;
extern mutex mtx_coop_mode;

extern std::jthread syncThread;

class Object {
private:
	int type;
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
	int bodyDirect1;
	int bodyDirect2;
	Vector2f pos;

	SnakeBody(int direct1, int direct2, Vector2f pos) {
		this->bodyDirect1 = direct1;
		this->bodyDirect2 = direct2;
		this->pos = pos;
	}
};

class Snake {
private:
	int direction = 0;
	int length;
	vector<SnakeBody> body;

public:
	Snake(int length, int direction, vector<SnakeBody>& body)
	{
		this->length = length;
		this->body = body;
		this->direction = direction;
	};
	~Snake() {
		while (!body.empty()) {
			body.pop_back();
		}
	};
	int getLength() {
		return length;
	}
	int getSize() {
		return body.size();
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
		int pxToDel = OBJECT_SIZE;
		int Index;
		if (length > 2) {
			int avToDel = 0;
			int currToDel = 0;
			while (pxToDel > 0) {
				Index = body.size() - 1;
				switch (body[Index].bodyDirect1) {
				case 0:
					avToDel = body[Index].pos.y - body[Index - 1].pos.y; break;
				case 1:
					avToDel = body[Index - 1].pos.y - body[Index].pos.y; break;
				case 2:
					avToDel = body[Index].pos.x - body[Index - 1].pos.x; break;
				case 3:
					avToDel = body[Index - 1].pos.x - body[Index + 1].pos.x; break;
				}
				if (avToDel > 0 && avToDel < pxToDel) {
					currToDel = avToDel;
					pxToDel -= avToDel;
					if (body.size() >= 3)
						body[Index].bodyDirect1 = body[Index - 1].bodyDirect1;
					    body[Index].bodyDirect2 = body[Index - 1].bodyDirect1;
						body[Index].pos = body[Index - 1].pos;
						body.erase(body.end() - 2);
				}
				else {
					currToDel = pxToDel;
					pxToDel = 0;
				}
				Index = body.size() - 1;
				switch (body[Index].bodyDirect1) {
				case 0:
					body[Index].pos.y -= currToDel; break;
				case 1:
					body[Index].pos.y += currToDel; break;
				case 2:
					body[Index].pos.x -= currToDel; break;
				case 3:
					body[Index].pos.x += currToDel; break;
				}

			}
			length -= 1;
		}
	}
};


class Player {
	public:
	int direction;
	Vector2f pos;
	Player() {
		this->direction = 0;
		this->pos = { 0, 0 };
	}
	Player(int direction, float posX, float posY) {
		this->direction = direction;
		this->pos = { posX, posY };
	}
};

class Package {
public:
	int game_status;
	int score;
	int bestScore;
	int playerID;
	int playerDirection;
	float playerPosX;
	float playerPosY;
	vector<Snake> snakes;
	vector<Vector2f> antidotes;
	map<int, Player> players;

	Package() {
		this->game_status = 0;
		this->score = 0;
		this->bestScore = 0;
		this->playerID = 10;
		this->playerDirection = 0;
		this->playerPosX = 0;
		this->playerPosY = 0;
		this->snakes = vector<Snake>();
		this->antidotes = vector<Vector2f>();
	}

	Package(int game_status,
		int score,
		int bestScore,
		int playerID,
		int playerDirection,
		float playerPosX,
		float playerPosY,
		vector<Snake> snakes,
		vector<Vector2f> antidotes) {
		this->game_status = game_status;
		this->score = score;
		this->bestScore = bestScore;
		this->playerID = playerID;
		this->playerDirection = playerDirection;
		this->playerPosX = playerPosX;
		this->playerPosY = playerPosY;
		this->snakes = vector<Snake>(snakes);
		for (int i = 0; i < snakes.size(); i++) {
			this->snakes[i].getBody() = vector<SnakeBody>(snakes[i].getBody());
		}
		this->antidotes = vector<Vector2f>(antidotes);
		for (int i = 0; i < antidotes.size(); i++) {
			this->antidotes[i] = Vector2f(antidotes[i]);
		}
	}

	~Package() {
		snakes.clear();
		players.clear();
		antidotes.clear();
	}
};

extern vector<Object> objects;
extern vector<Snake> snakes;
extern vector<Vector2f> antidotes;
extern vector<int> hittedSnakes;
extern vector<Vector2f> usedAntidotes;
extern map<int, Player> players;

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
extern Sprite* CoopButtonSprite;
extern Sprite* ExitButtonSprite;
extern Sprite* ContinueButtonSprite;
extern Sprite* RestartButtonSprite;
extern Sprite* MenuButtonSprite;
extern Sprite* LogoSprite;
extern Sprite* GameOverSprite;
extern Sprite* AntidoteSprite;
extern Sprite* SkullSprite;
extern Sprite* SnakeBodyBendSprite;

extern Texture PlayerFrontTexture;
extern Texture PlayerBackTexture;
extern Texture PlayerLeftTexture;
extern Texture PlayerRightTexture;
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
extern Texture SkullTexture;
extern Texture AntidoteTexture;

extern Text* textScore;
extern Text* textBestScore;
extern Text* timeToDeath;

extern Clock* poisonClock;

extern View view;

//FUNCTIONS

//Kutsenko Roman
void createServerThread();
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
bool createSock(const char* ipAddress);
void createServer(string* ipAddress);
void connectToServer(string* ipAddress, int code);
void sendToServer(Package *pckg);
Package* getFromServer();
void sendObjects();
void recvObjects();
void sendFloat(float f);
void sendInt(int32_t v);
void syncData(std::stop_token stoken);
void createSendPackage();
void getRecvPackage();
void drawPlayers(RenderWindow* window);
int InputConnectBox(RenderWindow* window, Text* IP, Text* code, int* inputType);
void processInputEvent(optional<Event>* event, Text* drawableText, string* str);

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
void lowerMusic(float tgVolume = 20.0f);
void restoreMusic(float defaultVolume = 45.0f);
extern sf::Music deathMusic;
bool DeathMusicFunc();
void PlayDeathMusic();
void StopDeathMusic();
extern sf::Music menuMusic;
bool MenuMusicFunc();
void PlayMenuMusic();
void StopMenuMusic();

//Vika Pavlenko
void showStartMenu(RenderWindow* window);
void showPauseMenu(RenderWindow* window);
void showDeathScreen(RenderWindow* window);