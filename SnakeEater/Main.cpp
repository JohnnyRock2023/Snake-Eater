#include "Header.h"

int game_status = 1;

// Objects
vector<Object> objects;
RectangleShape GrassShape;
RectangleShape RockShape;
RectangleShape StumpShape;
RectangleShape BushShape;
RectangleShape Player;

float playerPosX = (MAP_SIZEX / 2) - (OBJECT_SIZE / 2);
float playerPosY = (MAP_SIZEY / 2) - (OBJECT_SIZE / 2);
float viewPosX = (MAP_SIZEX / 2);
float viewPosY = (MAP_SIZEY / 2);

void renderingThread(RenderWindow* window)
{
    srand(time(NULL));
    View view({ viewPosX, viewPosY }, {SCREEN_RESX, SCREEN_RESY});
    window->setView(view);
    window->setActive(true);
    setObjectsPos();
    Clock clock;
    float timer = 0;

    while (window->isOpen())
    {
        if (game_status == 1) {
            float time = clock.getElapsedTime().asMilliseconds();
            clock.restart();
            timer += time;

            if (timer > SPEED) {
                timer = 0;

                if ((Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up)) && playerPosY > 0) {
                    if (viewPosY - (SCREEN_RESY / 2) > 0 && playerPosY + (PLAYER_SIZEY / 2) == viewPosY) {
                        viewPosY -= STEP;
                    }
                    playerPosY -= STEP;
                }
                else if ((Keyboard::isKeyPressed(Keyboard::Key::S) || Keyboard::isKeyPressed(Keyboard::Key::Down)) && playerPosY < MAP_SIZEY - PLAYER_SIZEY) {
                    if (viewPosY + (SCREEN_RESY / 2) < MAP_SIZEY && playerPosY + (PLAYER_SIZEY / 2) == viewPosY) {
                        viewPosY += STEP;
                    }
                    playerPosY += STEP;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Key::A) || Keyboard::isKeyPressed(Keyboard::Key::Left)) && playerPosX > 0) {
                    if (viewPosX - (SCREEN_RESX / 2) > 0 && playerPosX + (PLAYER_SIZEX / 2) == viewPosX) {
                        viewPosX -= STEP;
                    }
                    playerPosX -= STEP;
                }
                else if ((Keyboard::isKeyPressed(Keyboard::Key::D) || Keyboard::isKeyPressed(Keyboard::Key::Right)) && playerPosX < MAP_SIZEX - PLAYER_SIZEX) {
                    if (viewPosX + (SCREEN_RESX / 2) < MAP_SIZEX && playerPosX + (PLAYER_SIZEX / 2) == viewPosX) {
                        viewPosX += STEP;
                    }
                    playerPosX += STEP;
                }
                Player.setPosition({playerPosX, playerPosY});
                view.setCenter({ viewPosX, viewPosY });
                window->setView(view);
            }
        }
        window->clear(Color::Green);
        fillTheMapWithObj(window);
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


    GrassShape.setFillColor(Color(34, 148, 106, 255));
    GrassShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    RockShape.setFillColor(Color(204, 6, 5, 255));
    RockShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    StumpShape.setFillColor(Color(20, 89, 201, 255));
    StumpShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    BushShape.setFillColor(Color(237, 204, 32, 255));
    BushShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    Player.setFillColor(Color(255, 255, 255, 255));
    Player.setSize(Vector2f(PLAYER_SIZEX, PLAYER_SIZEY));
    Player.setPosition({playerPosX, playerPosY});

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