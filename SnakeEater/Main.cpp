#include "Header.h"

int game_status = 0;
int coop_mode = 0;
int score = 0;
int bestScore = 0;

int inputType = 0;
bool isPoisoned = false;

float SCREEN_RESX = 1280;
float SCREEN_RESY = 720;

float timer = 0;
float snakeTimer = 0;
float attackTimer = 0;
float poisonTimer = 0;
float pauseTimer = 0;

string IPInput = "192.168.1.100";
string codeInput = "";

vector<Object> objects;
vector<Snake> snakes;
vector<Vector2f> antidotes;
vector<int> hittedSnakes = vector<int>(NUM_OF_SNAKES, 0);
vector<Vector2f> usedAntidotes = vector<Vector2f>();

Texture PlayerFrontTexture;
Texture PlayerBackTexture;
Texture PlayerLeftTexture;
Texture PlayerRightTexture;
Texture SnakeBodyTexture;
Texture SnakeBodyBendTexture;
Texture SnakeHeadTexture;
Texture SnakeTailTexture;
Texture RockLTexture;
Texture RockSTexture;
Texture BushLTexture;
Texture BushSTexture;
Texture GrassTexture;
Texture StumpTexture;
Texture GroundTexture;
Texture StartButtonTexture;
Texture ExitButtonTexture;
Texture ContinueButtonTexture;
Texture RestartButtonTexture;
Texture MenuButtonTexture;
Texture LogoTexture;
Texture GameOverTexture;
Texture AntidoteTexture;
Texture SkullTexture;

Text* textScore = nullptr;
Text* textBestScore = nullptr;
Text* timeToDeath = nullptr;

Text* IPText = nullptr;
Text* codeText = nullptr;
Text* inviteText = nullptr;
Font font;

Sprite* GrassSprite = nullptr;
Sprite* RockSprite = nullptr;
Sprite* StumpSprite = nullptr;
Sprite* BushSprite = nullptr;
Sprite* PlayerSprite = nullptr;
Sprite* SnakeBodySprite = nullptr;
Sprite* SnakeBodyBendSprite = nullptr;
Sprite* SnakeHeadSprite = nullptr;
Sprite* SnakeTailSprite = nullptr;
Sprite* GroundSprite = nullptr;
Sprite* StartButtonSprite = nullptr;
Sprite* CoopButtonSprite = nullptr;
Sprite* ExitButtonSprite = nullptr;
Sprite* ContinueButtonSprite = nullptr;
Sprite* RestartButtonSprite = nullptr;
Sprite* MenuButtonSprite = nullptr;
Sprite* LogoSprite = nullptr;
Sprite* GameOverSprite = nullptr;
Sprite* AntidoteSprite = nullptr;
Sprite* SkullSprite = nullptr;
Clock* poisonClock = nullptr;

mutex mtx1;
mutex mtx2;
mutex mtx_game_status;
mutex mtx_coop_mode;

short int playerDirection = 1;
float playerPosX = (MAP_SIZEX / 2);
float playerPosY = (MAP_SIZEY / 2);
float viewPosX = (MAP_SIZEX / 2);
float viewPosY = (MAP_SIZEY / 2);

std::jthread syncThread;
std::jthread connToServ;

void openConsole() {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
}

void createServerThread() {
	static Clock threadTry;
	mtx_coop_mode.lock();
	if (coop_mode == 1 && threadTry.getElapsedTime().asMilliseconds() > 3000) {
		threadTry.restart();
		connToServ = std::jthread(&createServer, &IPInput);
	}
	else if (coop_mode == 2 && threadTry.getElapsedTime().asMilliseconds() > 3000) {
		threadTry.restart();
		connToServ = std::jthread(&connectToServer, &IPInput, stoi(codeInput));
	}
	else if (coop_mode == 3) {
		coop_mode = 4;
		try {
			syncThread = std::jthread(&syncData);
		}
		catch (...) {
			MessageBoxA(NULL, "Не вдалося створити потік синхронізації даних", "Помилка", MB_OK | MB_ICONERROR);
		}
	}
	mtx_coop_mode.unlock();
}

View view({ viewPosX, viewPosY }, { SCREEN_RESX, SCREEN_RESY });

void renderingThread(RenderWindow* window)
{
	MenuMusicFunc();
	DeathMusicFunc();
	TimerSoundFunc();
	SnakeBiteSoundFunc();
	AntidoteSoundFunc();
	ButtonClickSoundFunc();
	HitSnakeSound();
	FootStepSound();
	AudioTrack();
	srand(time(NULL));
	window->setView(view);
	window->setActive(true);
	setObjectsPos();
	setAntidotesPos(MAX_NUM_OF_ANTIDOTES);
	spawnSnakes(NUM_OF_SNAKES);

	Clock clock;
	Clock snakeClock;
	Clock attackClock;
	Clock pauseClock;
	poisonClock = new Clock;

	//openConsole();
	while (window->isOpen())
	{
		float pauseTime = pauseClock.getElapsedTime().asMilliseconds();
		pauseClock.restart();
		pauseTimer += pauseTime;

		if ((game_status == 1 && pauseTimer > 300 && Keyboard::isKeyPressed(Keyboard::Key::Escape)) && (!coop_mode || !playerID)) {
				game_status = 2;
				pauseTimer = 0;
		}
		if (coop_mode == 4) {
			getRecvPackage();
		}
		if (game_status == 1) {
			float time = clock.getElapsedTime().asMilliseconds();
			clock.restart();
			timer += time;

			float attackTime = attackClock.getElapsedTime().asMilliseconds();
			attackClock.restart();
			attackTimer += attackTime;

			if (timer > SPEED) {
				timer = 0;

				if ((Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up)) && playerPosY > PLAYER_SIZEY / 2) {
					if (viewPosY - (SCREEN_RESY / 2) > 0 && playerPosY == viewPosY) {
						viewPosY -= STEP;
					}
					playerPosY -= STEP;
					PlayFootStepSound();
					playerDirection = 0;
				}
				else if ((Keyboard::isKeyPressed(Keyboard::Key::S) || Keyboard::isKeyPressed(Keyboard::Key::Down)) && playerPosY < MAP_SIZEY - PLAYER_SIZEY / 2) {
					if (viewPosY + (SCREEN_RESY / 2) < MAP_SIZEY && playerPosY == viewPosY) {
						viewPosY += STEP;
					}
					playerPosY += STEP;
					PlayFootStepSound();
					playerDirection = 1;
				}
				if ((Keyboard::isKeyPressed(Keyboard::Key::A) || Keyboard::isKeyPressed(Keyboard::Key::Left)) && playerPosX > PLAYER_SIZEY / 2) {
					if (viewPosX - (SCREEN_RESX / 2) > 0 && playerPosX == viewPosX) {
						viewPosX -= STEP;
					}
					playerPosX -= STEP;
					PlayFootStepSound();
					playerDirection = 2;
				}
				else if ((Keyboard::isKeyPressed(Keyboard::Key::D) || Keyboard::isKeyPressed(Keyboard::Key::Right)) && playerPosX < MAP_SIZEX - PLAYER_SIZEX / 2) {
					if (viewPosX + (SCREEN_RESX / 2) < MAP_SIZEX && playerPosX == viewPosX) {
						viewPosX += STEP;
					}
					playerPosX += STEP;
					PlayFootStepSound();
					playerDirection = 3;
				}

			}

			if (attackTimer > HIT_DELAY) {
				if (Mouse::isButtonPressed(Mouse::Button::Left) || Keyboard::isKeyPressed(Keyboard::Key::Space)) {
					attackTimer = 0;
					attackSnake(playerDirection);
				}
			}

			if (isPoisoned) {
				float poisonTime = poisonClock->getElapsedTime().asSeconds();
				poisonClock->restart();
				poisonTimer += poisonTime;
			}

			if (poisonTimer > DEATH) {
				if (coop_mode || playerID > 0) {
					game_status = 0;
					coop_mode = 0;
					syncThread.request_stop();
					closesocket(clientSock);
					restart();
				}
				else {
					game_status = 3;
				}
			}
		}

		if (game_status == 1 || game_status == 4 || game_status == 0 || game_status == 3) {
			float snakeTime = snakeClock.getElapsedTime().asMilliseconds();
			snakeClock.restart();
			snakeTimer += snakeTime;

			if ((coop_mode == 0 || playerID == 0) && snakeTimer > SNAKE_SPEED) {
				snakeTimer = 0;
				moveSnakes();
			}
			createServerThread();
			handleZoom(view);
			if (viewPosY - (SCREEN_RESY / 2) > 0 && viewPosY + (SCREEN_RESY / 2) < MAP_SIZEY) {
				viewPosY += (playerPosY - viewPosY) * 0.15f;
			}
			if (viewPosX + (SCREEN_RESX / 2) < MAP_SIZEX && viewPosX - (SCREEN_RESX / 2) > 0) {
				viewPosX += (playerPosX - viewPosX) * 0.15f;
			}
			view.setCenter({ viewPosX, viewPosY });
			window->setView(view);
			drawGround(window);
			fillTheMapWithObj(window);
			drawAntidotes(window);
			deleteSnakes();
			drawSnakes(window);
			if (coop_mode && inviteCode != NULL) {
				inviteText->setPosition({ viewPosX + SCREEN_RESX / 2 - 300, viewPosY - SCREEN_RESY / 2 + 30 });
				inviteText->setString("INVITE CODE: " + to_string(inviteCode));
				window->draw(*inviteText);
			}

			if (game_status == 1) {
				drawPlayers(window);
				PlayerSprite->setPosition({ playerPosX, playerPosY });
				switch (playerDirection) {
				case 0:
					PlayerSprite->setTexture(PlayerBackTexture, false); break;
				case 1:
					PlayerSprite->setTexture(PlayerFrontTexture, false); break;
				case 2:
					PlayerSprite->setTexture(PlayerLeftTexture, false); break;
				case 3:
					PlayerSprite->setTexture(PlayerRightTexture, false); break;
				}
				window->draw(*PlayerSprite);
				if (snakes.size() <= 1) {
					spawnSnakes(NUM_OF_ADDITIONAL_SNAKES);
					setAntidotesPos(NUM_OF_ADDITIONAL_ANTIDOTES);
				}
				if (!isPoisoned) {
					snakeBite();
				}
				if (isPoisoned) {
					useAntidote();
				}
			}
			if (game_status == 1 || game_status == 2) {
				displayScore(window);
				displayTimeToDeath(window);
			}
		}

		if (game_status == 0) {
			showStartMenu(window);
		}
		if (game_status == 2) {
			showPauseMenu(window);
		}
		if (game_status == 3) {
			showDeathScreen(window);
		}
		if (game_status == 4) {
			int num = InputConnectBox(window, IPText, codeText, &inputType);
			switch (num) {
				case 1: 
					mtx_coop_mode.lock();
					coop_mode = 1;
					cout << coop_mode << endl;
					mtx_coop_mode.unlock();
					break;
				case 2: 
					mtx_coop_mode.lock();
					coop_mode = 2;
					cout << coop_mode << endl;
					mtx_coop_mode.unlock();
					break;
			}
		}
		window->display();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	ContextSettings settings;
	settings.antiAliasingLevel = 4;

	RenderWindow window(VideoMode({ (unsigned int)SCREEN_RESX, (unsigned int)SCREEN_RESY }), "Snake Eater", State::Windowed, settings);

	window.setActive(false);
	window.setFramerateLimit(120);
	Image logo;
	logo.loadFromFile("SnakeLogo.png");
	window.setIcon(logo);

	if (!PlayerFrontTexture.loadFromFile("Images/Front-1.png", false)) {}
	if (!PlayerBackTexture.loadFromFile("Images/Back-3.png", false)) {}
	if (!PlayerLeftTexture.loadFromFile("Images/Left-2.png", false)) {}
	if (!PlayerRightTexture.loadFromFile("Images/Right-2.png", false)) {}
	if (!SnakeBodyTexture.loadFromFile("Images/body.png", false)) {}
	if (!SnakeBodyBendTexture.loadFromFile("Images/bend.png", false)) {}
	if (!SnakeHeadTexture.loadFromFile("Images/head.png", false)) {}
	if (!SnakeTailTexture.loadFromFile("Images/tail.png", false)) {}
	if (!BushLTexture.loadFromFile("Images/BushL.png", false)) {}
	if (!BushSTexture.loadFromFile("Images/BushS.png", false)) {}
	if (!RockLTexture.loadFromFile("Images/RockL.png", false)) {}
	if (!RockSTexture.loadFromFile("Images/RockS.png", false)) {}
	if (!GrassTexture.loadFromFile("Images/Grass.png", false)) {}
	if (!StumpTexture.loadFromFile("Images/Stump.png", false)) {}
	if (!GroundTexture.loadFromFile("Images/Overworld.png", false)) {}
	if (!StartButtonTexture.loadFromFile("Images/Play.png", false)) {}
	if (!ExitButtonTexture.loadFromFile("Images/Exit.png", false)) {}
	if (!ContinueButtonTexture.loadFromFile("Images/Continue.png", false)) {}
	if (!RestartButtonTexture.loadFromFile("Images/Restart.png", false)) {}
	if (!MenuButtonTexture.loadFromFile("Images/Menu.png", false)) {}
	if (!LogoTexture.loadFromFile("Images/Logo.png", false)) {}
	if (!GameOverTexture.loadFromFile("Images/GameOver.png", false)) {}
	if (!AntidoteTexture.loadFromFile("Images/Antidote.png", false)) {}
	if (!SkullTexture.loadFromFile("Images/Skull.png", false)) {}

	if (!font.openFromFile("Fonts/Arial.ttf")) {}

	IPText = new Text(font);
	codeText = new Text(font);
	textScore = new Text(font);
	textBestScore = new Text(font);
	timeToDeath = new Text(font);
	inviteText = new Text(font);

	textScore->setCharacterSize(30);
	textBestScore->setCharacterSize(30);
	timeToDeath->setCharacterSize(30);
	textScore->setFillColor(Color::Black);
	textBestScore->setFillColor(Color::Black);
	timeToDeath->setFillColor(Color::Black);
	IPText->setCharacterSize(34);
	IPText->setFillColor(sf::Color::Black);
	codeText->setCharacterSize(34);
	codeText->setFillColor(sf::Color::Black);
	inviteText->setCharacterSize(26);
	inviteText->setFillColor(sf::Color::Black);

	GrassSprite = new Sprite(GrassTexture);
	RockSprite = new Sprite(RockLTexture);
	StumpSprite = new Sprite(StumpTexture);
	BushSprite = new Sprite(BushLTexture);
	PlayerSprite = new Sprite(PlayerFrontTexture);
	SnakeBodySprite = new Sprite(SnakeBodyTexture);
	SnakeHeadSprite = new Sprite(SnakeHeadTexture);
	SnakeTailSprite = new Sprite(SnakeTailTexture);
	GroundSprite = new Sprite(GroundTexture);
	StartButtonSprite = new Sprite(StartButtonTexture);
	CoopButtonSprite = new Sprite(StartButtonTexture);
	ExitButtonSprite = new Sprite(ExitButtonTexture);
	ContinueButtonSprite = new Sprite(ContinueButtonTexture);
	RestartButtonSprite = new Sprite(RestartButtonTexture);
	MenuButtonSprite = new Sprite(MenuButtonTexture);
	LogoSprite = new Sprite(LogoTexture);
	GameOverSprite = new Sprite(GameOverTexture);
	AntidoteSprite = new Sprite(AntidoteTexture);
	SkullSprite = new Sprite(SkullTexture);
	SnakeBodyBendSprite = new Sprite(SnakeBodyBendTexture);

	SnakeBodySprite->setOrigin({ OBJECT_SIZE / 2, 1.5f });
	SnakeBodyBendSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
	SnakeHeadSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
	SnakeTailSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
	PlayerSprite->setOrigin({ PLAYER_SIZEX / 2, PLAYER_SIZEY / 2 });
	AntidoteSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
	SkullSprite->setOrigin({ (float)SkullTexture.getSize().x / 2, (float)SkullTexture.getSize().y / 2 });

	std::thread thread(&renderingThread, &window);

	while (window.isOpen())
	{
		while (optional event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
				window.close();
			if (event->is<Event::Resized>()) {
				SCREEN_RESX = window.getSize().x;
				SCREEN_RESY = window.getSize().y;
				view.setSize({ SCREEN_RESX, SCREEN_RESY });
			}
			if (game_status == 4) {
				switch (inputType) {
				case 1:
					processInputEvent(&event, IPText, &IPInput);
					break;
				case 2:
					processInputEvent(&event, codeText, &codeInput);
					break;
				}
			}
		}
	}
	if (thread.joinable()) {
	thread.join();
	}
	if (connToServ.joinable()) {
		connToServ.join();
	}
	if (syncThread.joinable()) {
		syncThread.request_stop();
	}
	WSACleanup();
	return 0;
}