#include "Header.h"

void handleZoom(sf::View& view) {
    if (game_status != 1) return;

    static float currentZoom = 1.f;
    static float targetZoom = 1.f;
    static const float zoomInFactor = 0.6f;
    static const float zoomSpeed = 4.0f;
    static sf::Clock zoomClock;

    float deltaTime = zoomClock.restart().asSeconds();

    bool isRightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    targetZoom = isRightPressed ? zoomInFactor : 1.f;

    if (std::abs(currentZoom - targetZoom) > 0.001f) {
        float factor = 1.f + (targetZoom - currentZoom) * zoomSpeed * deltaTime;
        view.zoom(factor);
        currentZoom *= factor;
    }

    Vector2f playerCenter(playerPosX, playerPosY);
    Vector2f halfView = view.getSize() / 2.f;

    float minX = halfView.x;
    float maxX = MAP_SIZEX - halfView.x;
    float minY = halfView.y;
    float maxY = MAP_SIZEY - halfView.y;

    Vector2f newCenter;
    newCenter.x = std::clamp(playerCenter.x, minX, maxX);
    newCenter.y = std::clamp(playerCenter.y, minY, maxY);

    viewPosX = newCenter.x;
    viewPosY = newCenter.y;
}