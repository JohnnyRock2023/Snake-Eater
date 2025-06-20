#include "Header.h"

int game_status = 0;
int score = 0;
bool isPoisoned = false;

float timer = 0;
float snakeTimer = 0;
float attackTimer = 0;
float poisonTimer = 0;

// Objects
vector<Object> objects;
vector<Snake> snakes;
vector<Vector2f> antidotes;

Texture PlayerFrontTexture;
Texture PlayerBackTexture;
Texture PlayerLeftTexture;
Texture PlayerRightTexture;
Texture SnakeBodyTexture;
Texture SnakeBodyBendTexture;
Texture SnakeHeadTexture;
Texture SnakeTailTexture;
Texture RockTexture;
Texture GrassTexture;
Texture StumpTexture;
Texture BushTexture;
Texture GroundTexture;
Texture StartButtonTexture;
Texture ExitButtonTexture;
Texture AntidoteTexture;

Sprite* GrassSprite = nullptr;
Sprite* RockSprite = nullptr;
Sprite* StumpSprite = nullptr;
Sprite* BushSprite = nullptr;
Sprite* PlayerSprite = nullptr;
Sprite* SnakeBodySprite = nullptr;
Sprite* SnakeHeadSprite = nullptr;
Sprite* SnakeTailSprite = nullptr;
Sprite* GroundSprite = nullptr;
Sprite* StartButtonSprite = nullptr;
Sprite* ExitButtonSprite = nullptr;
Sprite* AntidoteSprite = nullptr;

float playerPosX = (MAP_SIZEX / 2);
float playerPosY = (MAP_SIZEY / 2);
short int playerDirection = 1;
float viewPosX = (MAP_SIZEX / 2);
float viewPosY = (MAP_SIZEY / 2);

void renderingThread(RenderWindow* window)
{
    HitSnakeSound();
    FootStepSound();
    AudioTrack();
    srand(time(NULL));
    View view({ viewPosX, viewPosY }, { SCREEN_RESX, SCREEN_RESY });
    window->setView(view);
    window->setActive(true);
    setObjectsPos();
    setAntidotesPos();
    spawnSnakes(30);

    Clock clock;
    Clock snakeClock;
    Clock attackClock;
    Clock poisonClock;

    while (window->isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            if (game_status == 1) {
                game_status = 2;
            }
        }
        if (game_status == 1) {
            float time = clock.getElapsedTime().asMilliseconds();
            clock.restart();
            timer += time;
            float snakeTime = snakeClock.getElapsedTime().asMilliseconds();
            snakeClock.restart();
            snakeTimer += snakeTime;

            float attackTime = attackClock.getElapsedTime().asMilliseconds();
            attackClock.restart();
            attackTimer += attackTime;

            float poisonTime = poisonClock.getElapsedTime().asSeconds();
            poisonClock.restart();
            poisonTimer += poisonTime;

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
                view.setCenter({ viewPosX, viewPosY });
                window->setView(view);
            }

            if (snakeTimer > SNAKE_SPEED) {
                snakeTimer = 0;
                //moveSnakes();
            }

            if (attackTimer > HIT_DELAY) {
                if (Mouse::isButtonPressed(Mouse::Button::Left) || Keyboard::isKeyPressed(Keyboard::Key::Space)) {
                    attackTimer = 0;
                    attackSnake({ playerPosX, playerPosY }, playerDirection);
                }
            }

            if (isPoisoned) {
                poisonTimer += poisonTime;
            }

            if (poisonTimer > DEATH) {
                //window->setTitle("It's over!");
            }
        }
        drawGround(window);
        fillTheMapWithObj(window);
        drawAntidotes(window);
        deleteSnakes();
        drawSnakes(window);
        //window->setTitle("Snake Eater \t\t\t\t Score: " + to_string(score));
        window->setTitle(to_string(isPoisoned));
        handleZoom(view);
        window->draw(*PlayerSprite);
        snakeBite();
        if (game_status == 0) {
            showStartMenu(window);
        }
        if (game_status == 2) {
            View pauseView({ MAP_SIZEX / 2.f, MAP_SIZEY / 2.f }, { SCREEN_RESX, SCREEN_RESY });
            window->setView(pauseView);
            showPauseMenu(window);
        }
        if (isPoisoned) {
            useAntidote();
        }
        window->display();
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;

    RenderWindow window(VideoMode({ SCREEN_RESX, SCREEN_RESY }), "Snake Eater", State::Windowed, settings);

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

    if (!BushTexture.loadFromFile("Images/Bush.png", false)) {}
    if (!RockTexture.loadFromFile("Images/Rock.png", false)) {}
    if (!GrassTexture.loadFromFile("Images/Grass.png", false)) {}
    if (!StumpTexture.loadFromFile("Images/Stump.png", false)) {}
    if (!GroundTexture.loadFromFile("Images/Overworld.png", false)) {}
    if (!StartButtonTexture.loadFromFile("Images/Play.png", false)) {}
    if (!ExitButtonTexture.loadFromFile("Images/Quit.png", false)) {}
    if (!AntidoteTexture.loadFromFile("Images/Antidote.png", false)) {}

    GrassSprite = new Sprite(GrassTexture);
    RockSprite = new Sprite(RockTexture);
    StumpSprite = new Sprite(StumpTexture);
    BushSprite = new Sprite(BushTexture);
    PlayerSprite = new Sprite(PlayerFrontTexture);
    SnakeBodySprite = new Sprite(SnakeBodyTexture);
    SnakeHeadSprite = new Sprite(SnakeHeadTexture);
    SnakeTailSprite = new Sprite(SnakeTailTexture);
    GroundSprite = new Sprite(GroundTexture);
    StartButtonSprite = new Sprite(StartButtonTexture);
    ExitButtonSprite = new Sprite(ExitButtonTexture);
    AntidoteSprite = new Sprite(AntidoteTexture);

    SnakeBodySprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
    SnakeHeadSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
    SnakeTailSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });
    PlayerSprite->setOrigin({ PLAYER_SIZEX / 2, PLAYER_SIZEY / 2 });
    AntidoteSprite->setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });

    thread thread(&renderingThread, &window);

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
    }
    thread.join();
    return 0;
}