#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <Windows.h>
using namespace sf;
using namespace std;


void renderingThread(RenderWindow* window)
{
    window->setActive(true);
    CircleShape shape(200.f, 60);
    shape.setFillColor(Color::Cyan);
    shape.setPosition({ 200, 200 });

    // the rendering loop
    while (window->isOpen())
    {
        window->clear(Color::Blue);
        window->draw(shape);
        window->display();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(VideoMode({ 800, 800 }), "Snake Eater", State::Windowed, settings);
    window.setActive(false);
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