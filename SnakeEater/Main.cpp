#include "Header.h"
//hg

int game_status = 1;

// Objects
vector<Object> objects;
vector<Snake> snakes;
RectangleShape GrassShape;
RectangleShape RockShape;
RectangleShape StumpShape;
RectangleShape BushShape;
RectangleShape Player;
RectangleShape SnakeBodyShape;
RectangleShape SnakeHeadShape;
RectangleShape SnakeTailShape;

Texture PlayerFrontTexture;
Texture SnakeBodyTexture;
Texture SnakeBodyBendTexture;
Texture SnakeHeadTexture;
Texture SnakeTailTexture;


float playerPosX = (MAP_SIZEX / 2) - (PLAYER_SIZEX / 2);
float playerPosY = (MAP_SIZEY / 2) - (PLAYER_SIZEY / 2);
short int playerDirection = 1;
float viewPosX = (MAP_SIZEX / 2);
float viewPosY = (MAP_SIZEY / 2);

void renderingThread(RenderWindow* window)
{
    srand(time(NULL));
    View view({ viewPosX, viewPosY }, {SCREEN_RESX, SCREEN_RESY});
    window->setView(view);
    window->setActive(true);
    setObjectsPos();
    spawnSnakes(10);
    Clock clock;
    Clock snakeClock;
    float timer = 0;
    float snakeTimer = 0;

    while (window->isOpen())
    {
        if (game_status == 1) {
            float time = clock.getElapsedTime().asMilliseconds();
            clock.restart();
            timer += time;
            float snakeTime = snakeClock.getElapsedTime().asMilliseconds();
            snakeClock.restart();
            snakeTimer += snakeTime;

            if (timer > SPEED) {
                timer = 0;

                if ((Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up)) && playerPosY > 0) {
                    if (viewPosY - (SCREEN_RESY / 2) > 0 && playerPosY + (PLAYER_SIZEY / 2) == viewPosY) {
                        viewPosY -= STEP;
                    }
                    playerPosY -= STEP;
                    playerDirection = 0;
                }
                else if ((Keyboard::isKeyPressed(Keyboard::Key::S) || Keyboard::isKeyPressed(Keyboard::Key::Down)) && playerPosY < MAP_SIZEY - PLAYER_SIZEY) {
                    if (viewPosY + (SCREEN_RESY / 2) < MAP_SIZEY && playerPosY + (PLAYER_SIZEY / 2) == viewPosY) {
                        viewPosY += STEP;
                    }
                    playerPosY += STEP;
                    playerDirection = 1;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Key::A) || Keyboard::isKeyPressed(Keyboard::Key::Left)) && playerPosX > 0) {
                    if (viewPosX - (SCREEN_RESX / 2) > 0 && playerPosX + (PLAYER_SIZEX / 2) == viewPosX) {
                        viewPosX -= STEP;
                    }
                    playerPosX -= STEP;
                    playerDirection = 2;
                }
                else if ((Keyboard::isKeyPressed(Keyboard::Key::D) || Keyboard::isKeyPressed(Keyboard::Key::Right)) && playerPosX < MAP_SIZEX - PLAYER_SIZEX) {
                    if (viewPosX + (SCREEN_RESX / 2) < MAP_SIZEX && playerPosX + (PLAYER_SIZEX / 2) == viewPosX) {
                        viewPosX += STEP;
                    }
                    playerPosX += STEP;
                    playerDirection = 3;
                }
                Player.setPosition({playerPosX, playerPosY});
                view.setCenter({ viewPosX, viewPosY });
                window->setView(view);
            }

            if (snakeTimer > SNAKE_SPEED) {
                snakeTimer = 0;
                moveSnakes();
            }
        }
        window->clear(Color::Green);
        fillTheMapWithObj(window);
        drawSnakes(window);
        window->setTitle(to_string(snakes.size()));
        window->draw(Player); 
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

    if (!SnakeBodyTexture.loadFromFile("Images/SnakeBody.png", false)) {}

    if (!SnakeBodyBendTexture.loadFromFile("Images/SnakeBodyBend.png", false)) {}

    if (!SnakeHeadTexture.loadFromFile("Images/SnakeHead.png", false)) {}
    if (!SnakeTailTexture.loadFromFile("Images/SnakeTail.png", false)) {}


    GrassShape.setFillColor(Color(34, 148, 106, 255));
    GrassShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    RockShape.setFillColor(Color(204, 6, 5, 255));
    RockShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    StumpShape.setFillColor(Color(20, 89, 201, 255));
    StumpShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    BushShape.setFillColor(Color(237, 204, 32, 255));
    BushShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    Player.setTexture(&PlayerFrontTexture, false);
    Player.setSize(Vector2f(PLAYER_SIZEX, PLAYER_SIZEY));
    Player.setPosition({playerPosX, playerPosY});

    SnakeBodyShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    SnakeBodyShape.setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });

    SnakeHeadShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    SnakeHeadShape.setTexture(&SnakeHeadTexture, false);
    SnakeHeadShape.setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });

    SnakeTailShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    SnakeTailShape.setTexture(&SnakeTailTexture, false);
    SnakeTailShape.setOrigin({ OBJECT_SIZE / 2, OBJECT_SIZE / 2 });

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