#include "Header.h"

float posForButtonX = viewPosX - SCREEN_RESX / 2;
float posForButtonY = viewPosY - SCREEN_RESY / 2;
float startButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float startButtonPosY = viewPosY + 20;

float createServerButtonPosX = viewPosX + BUTTON_WIDTH;
float createServerButtonPosY = viewPosY + 20;

float connectServerButtonPosX = viewPosX + BUTTON_WIDTH;
float connectServerButtonPosY = viewPosY +130;

float exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float exitButtonPosY = viewPosY + 130;
float pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
float continueButtonY = viewPosY - 100;
float restartButtonY = viewPosY;
float menuButtonY = viewPosY + 100;

void showStartMenu(RenderWindow* window) {

	static bool menuPl = false;
	if (!menuPl) {
		PlayMenuMusic();
		menuPl = true;
	}

	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	startButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	startButtonPosY = viewPosY + 20;
	exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	exitButtonPosY = viewPosY + 130;

	StartButtonSprite->setPosition({ startButtonPosX, startButtonPosY });
	ExitButtonSprite->setPosition({ exitButtonPosX, exitButtonPosY });
	LogoSprite->setPosition({ viewPosX - LogoTexture.getSize().x / 2,  viewPosY - 250 });

	window->draw(*LogoSprite);
	window->draw(*StartButtonSprite);
	window->draw(*ExitButtonSprite);

	StartButtonSprite->setPosition({ createServerButtonPosX, createServerButtonPosY });
	ExitButtonSprite->setPosition({ connectServerButtonPosX, connectServerButtonPosY });

	window->draw(*StartButtonSprite);
	window->draw(*ExitButtonSprite);


	if (pauseTimer > 300 && Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		// Натиск на кнопку старт
		if (inRange(posForButtonX + mousePos.x, startButtonPosX, startButtonPosX + BUTTON_WIDTH) && 
			inRange(posForButtonY + mousePos.y, startButtonPosY, startButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopMenuMusic();
			AudioTrack();
			restart();
			game_status = 1;
			coop_mode = 0;
			menuPl = false;
		}
		// Натиск на кнопку виходу
		if (inRange(posForButtonX + mousePos.x, exitButtonPosX, exitButtonPosX + BUTTON_WIDTH) && 
			inRange(posForButtonY + mousePos.y, exitButtonPosY, exitButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->close();
			
		}

		if (inRange(posForButtonX + mousePos.x, createServerButtonPosX, createServerButtonPosX + BUTTON_WIDTH) &&
			inRange(posForButtonY + mousePos.y, createServerButtonPosY, createServerButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			game_status = 1;
			coop_mode = 1;
			createServerThread();
			StopMenuMusic();
			AudioTrack();
			restart();
			menuPl = false;

		}

		if (inRange(posForButtonX + mousePos.x, connectServerButtonPosX, connectServerButtonPosX + BUTTON_WIDTH) &&
			inRange(posForButtonY + mousePos.y, connectServerButtonPosY, connectServerButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			game_status = 1;
			coop_mode = 2;
			createServerThread();
			StopMenuMusic();
			AudioTrack();
			snakes.clear();
			objects.clear();
			antidotes.clear();
			menuPl = false;
		}

	}
}

void showPauseMenu(RenderWindow* window) {

	lowerMusic();
	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
	continueButtonY = viewPosY;
	restartButtonY = viewPosY + 100;
	menuButtonY = viewPosY + 200;

	LogoSprite->setPosition({ viewPosX - 500.f, viewPosY - 250.f });
	ContinueButtonSprite->setPosition({ pauseButtonX, continueButtonY });
	RestartButtonSprite->setPosition({ pauseButtonX, restartButtonY });
	MenuButtonSprite->setPosition({ pauseButtonX, menuButtonY });

	window->draw(*LogoSprite);
	window->draw(*ContinueButtonSprite);
	window->draw(*RestartButtonSprite);
	window->draw(*MenuButtonSprite);

	if (pauseTimer > 300 && Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
		restoreMusic();
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
			restoreMusic();
			poisonClock->restart();
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			restoreMusic();
			restart();
			game_status = 1;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			restart();
			StopTimerSound();
			game_status = 0;
			pauseTimer = 0;
		}
	}
}

void showDeathScreen(RenderWindow* window) {

	static bool failMusic = false;
	if (!failMusic) {
		PlayDeathMusic();
		failMusic = true;
	}

	StopTimerSound();
	posForButtonX = viewPosX - SCREEN_RESX / 2;
	posForButtonY = viewPosY - SCREEN_RESY / 2;
	pauseButtonX = viewPosX - BUTTON_WIDTH / 2;
	restartButtonY = viewPosY + 50;
	menuButtonY = viewPosY + 160;

	GameOverSprite->setPosition({ viewPosX - 350.f, viewPosY - 270.f });
	SkullSprite->setPosition({ viewPosX,  viewPosY - 60 });
	RestartButtonSprite->setPosition({ pauseButtonX, restartButtonY });
	MenuButtonSprite->setPosition({ pauseButtonX, menuButtonY });
	textScore->setPosition({ viewPosX - textScore->getLocalBounds().getCenter().x, viewPosY - 10 });
	textScore->setString("YOUR SCORE: " + to_string(score));

	window->draw(*GameOverSprite);
	window->draw(*SkullSprite);
	window->draw(*textScore);
	window->draw(*RestartButtonSprite);
	window->draw(*MenuButtonSprite);

	if (game_status == 3 && Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		float mouseX = posForButtonX + mousePos.x;
		float mouseY = posForButtonY + mousePos.y;

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopDeathMusic();
			AudioTrack();
			restart();
			game_status = 1;
			failMusic = false;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopDeathMusic();
			PlayMenuMusic();
			restart();
			StopTimerSound();
			game_status = 0;
			pauseTimer = 0;
			failMusic = false;
		}
	}
}