#include "Header.h"

float posForButtonX = (MAP_SIZEX / 2) - SCREEN_RESX / 2;
float posForButtonY = (MAP_SIZEY / 2) - SCREEN_RESY / 2;
float startButtonPosX = MAP_SIZEX / 2 - BUTTON_WIDTH / 2;
float startButtonPosY = MAP_SIZEY / 2 + 20;
float exitButtonPosX = MAP_SIZEX / 2 - BUTTON_WIDTH / 2;
float exitButtonPosY = MAP_SIZEY / 2 + 130;
float pauseButtonX = MAP_SIZEX / 2 - BUTTON_WIDTH / 2;
float continueButtonY = MAP_SIZEY / 2 - 100;
float restartButtonY = MAP_SIZEY / 2;
float menuButtonY = MAP_SIZEY / 2 + 100;

void showStartMenu(RenderWindow* window) {
	StartButtonSprite->setPosition({ startButtonPosX, startButtonPosY });
	ExitButtonSprite->setPosition({ exitButtonPosX, exitButtonPosY });

	RectangleShape title({ 1000, 200 });
	title.setFillColor(Color::Red);
	title.setPosition({ MAP_SIZEX / 2 - title.getSize().x / 2,  MAP_SIZEY / 2 - 300 });

	window->draw(title);
	window->draw(*StartButtonSprite);
	window->draw(*ExitButtonSprite);

	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		// Натиск на кнопку старт
		if (inRange(posForButtonX + mousePos.x, startButtonPosX, startButtonPosX + BUTTON_WIDTH) && inRange(posForButtonY + mousePos.y, startButtonPosY, startButtonPosY + BUTTON_HEIGHT)) {
			window->setTitle("Start!");
			game_status = 1;
		}
		// Натиск на кнопку виходу
		if (inRange(posForButtonX + mousePos.x, exitButtonPosX, exitButtonPosX + BUTTON_WIDTH) && inRange(posForButtonY + mousePos.y, exitButtonPosY, exitButtonPosY + BUTTON_HEIGHT)) {
			window->close();
			window->setTitle("Exit!");
		}
	}
}

void showPauseMenu(RenderWindow* window) {
	RectangleShape titlePause({ 500.f, 100.f });
	titlePause.setFillColor(Color::White);
	titlePause.setPosition({ MAP_SIZEX / 2.f - 250.f, MAP_SIZEY / 2.f - 250.f });

	RectangleShape continueButton({ BUTTON_WIDTH, BUTTON_HEIGHT });
	continueButton.setFillColor(Color::Green);
	continueButton.setPosition({ pauseButtonX, continueButtonY });

	RectangleShape restartButton({ BUTTON_WIDTH, BUTTON_HEIGHT });
	restartButton.setFillColor(Color::Yellow);
	restartButton.setPosition({ pauseButtonX, restartButtonY });

	RectangleShape exitButton({ BUTTON_WIDTH, BUTTON_HEIGHT });
	exitButton.setFillColor(Color::Red);
    exitButton.setPosition({ pauseButtonX, menuButtonY });

	window->draw(titlePause);
	window->draw(continueButton);
	window->draw(restartButton);
	window->draw(exitButton);

	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		float mouseX = posForButtonX + mousePos.x;
		float mouseY = posForButtonY + mousePos.y;

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, continueButtonY, continueButtonY + BUTTON_HEIGHT)) {
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			window->setTitle("Restart clicked!");
			//функція для початку заново
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			window->setTitle("Menu clicked!");
		}
	}
}