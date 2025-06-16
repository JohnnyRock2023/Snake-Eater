#include <SFML/Graphics.hpp>
#include <Windows.h>
using namespace sf;
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(VideoMode({ 800, 800 }), "Snake Eater", State::Windowed, settings);
    CircleShape shape(200.f, 60);
    shape.setFillColor(Color::Cyan);
    shape.setPosition({ 200, 200 });

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        window.clear(Color::Blue);
        window.draw(shape);
        window.display();
    }
}