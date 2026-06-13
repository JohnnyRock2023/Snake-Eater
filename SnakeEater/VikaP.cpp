#include "Header.h"

float posForButtonX = viewPosX - SCREEN_RESX / 2;
float posForButtonY = viewPosY - SCREEN_RESY / 2;
float startButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float startButtonPosY = viewPosY + 20;

float coopButtonPosX = viewPosX - BUTTON_WIDTH/2;
float coopButtonPosY = viewPosY + 130;

float exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
float exitButtonPosY = viewPosY + 260;
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
	coopButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	coopButtonPosY = viewPosY + 130;
	exitButtonPosX = viewPosX - BUTTON_WIDTH / 2;
	exitButtonPosY = viewPosY + 240;

	StartButtonSprite->setPosition({ startButtonPosX, startButtonPosY });
	ExitButtonSprite->setPosition({ exitButtonPosX, exitButtonPosY });
	LogoSprite->setPosition({ viewPosX - LogoTexture.getSize().x / 2,  viewPosY - 250 });

	window->draw(*LogoSprite);
	window->draw(*StartButtonSprite);
	window->draw(*ExitButtonSprite);

	CoopButtonSprite->setPosition({ coopButtonPosX, coopButtonPosY });
	window->draw(*CoopButtonSprite);


	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);
		// Натиск на кнопку старт
		if (pauseTimer > 300 && inRange(posForButtonX + mousePos.x, startButtonPosX, startButtonPosX + BUTTON_WIDTH) &&
			inRange(posForButtonY + mousePos.y, startButtonPosY, startButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopMenuMusic();
			AudioTrack();
			restart();
			mtx_game_status.lock();
			game_status = 1;
			mtx_game_status.unlock();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
			menuPl = false;
			pauseTimer = 0;
		}
		// Натиск на кнопку виходу
		if (pauseTimer > 300 && inRange(posForButtonX + mousePos.x, exitButtonPosX, exitButtonPosX + BUTTON_WIDTH) &&
			inRange(posForButtonY + mousePos.y, exitButtonPosY, exitButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			window->close();
		}

		if (pauseTimer > 300 && inRange(posForButtonX + mousePos.x, coopButtonPosX, coopButtonPosX + BUTTON_WIDTH) &&
			inRange(posForButtonY + mousePos.y, coopButtonPosY, coopButtonPosY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			mtx_game_status.lock();
			game_status = 4;
			mtx_game_status.unlock();
			menuPl = false;
			pauseTimer = 0;
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
		mtx_game_status.lock();
		game_status = 1;
		mtx_game_status.unlock();
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
			mtx_game_status.lock();
			game_status = 1;
			mtx_game_status.unlock();
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, restartButtonY, restartButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			restoreMusic();
			mtx_game_status.lock();
			game_status = 1;
			mtx_game_status.unlock();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
			if (syncThread.joinable()) {
				syncThread.request_stop();
			}
			closesocket(clientSock);
			restart();
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopTimerSound();
			if (syncThread.joinable()) {
				syncThread.request_stop();
			}
			closesocket(clientSock);
			restart();
			mtx_game_status.lock();
			game_status = 0;
			mtx_game_status.unlock();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
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
			if (syncThread.joinable()) {
				syncThread.request_stop();
			}
			closesocket(clientSock);
			restart();
			mtx_game_status.lock();
			game_status = 1;
			mtx_game_status.unlock();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
			failMusic = false;
		}

		if (inRange(mouseX, pauseButtonX, pauseButtonX + BUTTON_WIDTH) &&
			inRange(mouseY, menuButtonY, menuButtonY + BUTTON_HEIGHT)) {
			PlayButtonClickSound();
			StopDeathMusic();
			PlayMenuMusic();
			StopTimerSound();
			if (syncThread.joinable()) {
				syncThread.request_stop();
			}
			closesocket(clientSock);
			restart();
			mtx_game_status.lock();
			game_status = 0;
			mtx_game_status.unlock();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
			pauseTimer = 0;
			failMusic = false;
		}
	}
}