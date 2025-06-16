#include "Header.h"

// Objects
vector<Object> objects;
RectangleShape GrassShape;
RectangleShape RockShape;
RectangleShape StumpShape;
RectangleShape BushShape;
RectangleShape Shape1;

void renderingThread(RenderWindow* window)
{
    srand(time(NULL));
    window->setActive(true);
    setObjectsPos();

    while (window->isOpen())
    {
        window->clear(Color::Green);
        fillTheMapWithObj(window);
        window->display();
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(VideoMode({ SCREEN_RESX, SCREEN_RESY }), "Snake Eater", State::Windowed, settings);
    window.setActive(false);
    thread thread(&renderingThread, &window);

    GrassShape.setFillColor(Color(34, 148, 106, 255));
    GrassShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    RockShape.setFillColor(Color(204, 6, 5, 255));
    RockShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    StumpShape.setFillColor(Color(20, 89, 201, 255));
    StumpShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    BushShape.setFillColor(Color(237, 204, 32, 255));
    BushShape.setSize(Vector2f(OBJECT_SIZE, OBJECT_SIZE));

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