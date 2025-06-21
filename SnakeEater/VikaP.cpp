#include "Header.h"

float posForButtonX = viewPosX - SCREEN_RESX / 2;
float posForButtonY = viewPosY - SCREEN_RESY / 2;
float startButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float startButtonPosY = viewPosY + 20;
float exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float exitButtonPosY = viewPosY + 130;
float pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
float continueButtonY = viewPosY - 100;
float restartButtonY = viewPosY;
float menuButtonY = viewPosY + 100;

void showStartMenu(RenderWindow* window) {

	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	startButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	startButtonPosY = viewPosY + 20;
	exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	exitButtonPosY = viewPosY + 130;

	StartButtonSprite->setPosition({ startButtonPosX, startButtonPosY });
	ExitButtonSprite->setPosition({ exitButtonPosX, exitButtonPosY });

	RectangleShape title({ 1000, 200 });
	title.setFillColor(Color::Red);
	title.setPosition({ viewPosX - title.getSize().x / 2,  viewPosY - 300 });

	window->draw(title);
	window->draw(*StartButtonSprite);
	window->draw(*ExitButtonSprite);

	if (pauseTimer > 300 && Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		// Натиск на кнопку старт
		if (inRange(posForButtonX + mousePos.x, startButtonPosX, startButtonPosX + BUTTON_WIDTH) && inRange(posForButtonY + mousePos.y, startButtonPosY, startButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->setTitle("Start!");
			game_status = 1;
		}
		// Натиск на кнопку виходу
		if (inRange(posForButtonX + mousePos.x, exitButtonPosX, exitButtonPosX + BUTTON_WIDTH) && inRange(posForButtonY + mousePos.y, exitButtonPosY, exitButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->close();
			window->setTitle("Exit!");
		}
	}
}

void showPauseMenu(RenderWindow* window) {

	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
	continueButtonY = viewPosY - 100;
	restartButtonY = viewPosY;
	menuButtonY = viewPosY + 100;

	RectangleShape titlePause({ 500.f, 100.f });
	titlePause.setFillColor(Color::White);
	titlePause.setPosition({ viewPosX - 250.f, viewPosY - 250.f });

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

	if (pauseTimer > 300 && Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
		game_status = 1;
		pauseTimer = 0;
		poisonClock->restart();
	}

	else if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		float mouseX = posForButtonX + mousePos.x;
		float mouseY = posForButtonY + mousePos.y;

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, continueButtonY, continueButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			poisonClock->restart();
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			window->setTitle("Restart clicked!");
			PlayButtonClickSound();
			restart();
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			window->setTitle("Menu clicked!");
			PlayButtonClickSound();
			restart();
			StopTimerSound();
			game_status = 0;
			pauseTimer = 0;
		}
	}
}

void showDeathScreen(RenderWindow* window) {

	StopTimerSound();
	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
	restartButtonY = viewPosY + 20;
	menuButtonY = viewPosY + 130;

	RectangleShape titleEnd({ 500.f, 100.f });
	titleEnd.setFillColor(Color::White);
	titleEnd.setPosition({ viewPosX - 250.f, viewPosY - 250.f });

	RectangleShape restartButton({ BUTTON_WIDTH, BUTTON_HEIGHT });
	restartButton.setFillColor(Color::Yellow);
	restartButton.setPosition({ pauseButtonX, restartButtonY });

	RectangleShape exitButton({ BUTTON_WIDTH, BUTTON_HEIGHT });
	exitButton.setFillColor(Color::Red);
	exitButton.setPosition({ pauseButtonX, menuButtonY });
	window->draw(titleEnd);
	window->draw(restartButton);
	window->draw(exitButton);

	if (game_status == 3 && Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		float mouseX = posForButtonX + mousePos.x;
		float mouseY = posForButtonY + mousePos.y;

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->setTitle("Restart clicked!");
			restart();
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->setTitle("Menu clicked!");
			restart();
			StopTimerSound();
			game_status = 0;
			pauseTimer = 0;
		}
	}
}