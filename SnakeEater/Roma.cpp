#include "Header.h"



void setObjectsPos() {
	float posX;
	float posY;
	short int type;
	int count = MIN_NUM_OF_OBJECTS + rand() % MAX_NUM_OF_OBJECTS;
	for (int i = 0; i < count; i++) {
		type = rand() % 6;
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;

		if (isObjectPosNew(posX, posY)) {
			objects.push_back(Object(type, posX, posY));
		}
		else
			i--;
	}
}

void fillTheMapWithObj(RenderWindow* window) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].getType() == 0) {
			GrassSprite->setPosition(objects[i].getPos());
			window->draw(*GrassSprite);
		}
		else if (objects[i].getType() == 1) {
			RockSprite->setPosition(objects[i].getPos());
			RockSprite->setTexture(RockLTexture);
			window->draw(*RockSprite);
		}
		else if (objects[i].getType() == 2) {
			RockSprite->setPosition(objects[i].getPos());
			RockSprite->setTexture(RockSTexture);
			window->draw(*RockSprite);
		}
		else if (objects[i].getType() == 3) {
			StumpSprite->setPosition(objects[i].getPos());
			window->draw(*StumpSprite);
		}
		else if (objects[i].getType() == 4) {
			BushSprite->setPosition(objects[i].getPos());
			BushSprite->setTexture(BushLTexture);
			window->draw(*BushSprite);
		}
		else if (objects[i].getType() == 5) {
			BushSprite->setPosition(objects[i].getPos());
			BushSprite->setTexture(BushSTexture);
			window->draw(*BushSprite);
		}
	}
}

bool isSnakesPosNew(Vector2f pos) {
	float posX;
	float posY;
	for (int i = 0; i < snakes.size(); i++) {
		posX = snakes[i].getBody()[0].pos.x;
		posY = snakes[i].getBody()[0].pos.y;
		if (abs(posX - pos.x) < OBJECT_SIZE && abs(posY - pos.y) < OBJECT_SIZE)
			return false;
	}
	return true;
}

bool isNearPlayer(float posX, float posY) {
	if (inRange(posX, playerPosX + PLAYER_SIZEX / 2 - OBJECT_SIZE * 4, playerPosX + PLAYER_SIZEX / 2 + OBJECT_SIZE * 4) &&
		inRange(posY, playerPosY + PLAYER_SIZEY / 2 - OBJECT_SIZE * 4, playerPosY + PLAYER_SIZEY / 2 + OBJECT_SIZE * 4)) {
		return true;
	}
	return false;
}

void spawnSnakes(int count) {
	float posX = 0;
	float posY = 0;
	short int direction = 0;
	short int size = 0;
	for (int i = 0; i < count; i++) {
		direction = rand() % 4;
		size = MIN_SNAKE_SIZE + rand() % (MAX_SNAKE_SIZE - MIN_SNAKE_SIZE + 1);
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;

		while (!isSnakesPosNew({ posX, posY }) || isNearPlayer(posX, posY)) {
			posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
			posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;
		}

		vector<SnakeBody> body;
		body.push_back(SnakeBody(direction, direction, { posX, posY }));

		switch (direction) {
		case 0: body.push_back(SnakeBody(direction, direction, { posX, posY + size * OBJECT_SIZE })); break;
		case 1: body.push_back(SnakeBody(direction, direction, { posX, posY - size * OBJECT_SIZE })); break;
		case 2: body.push_back(SnakeBody(direction, direction, { posX + size * OBJECT_SIZE, posY })); break;
		case 3: body.push_back(SnakeBody(direction, direction, { posX - size * OBJECT_SIZE, posY })); break;
		}
		snakes.push_back(Snake(size, direction, body));
	}
}

void moveSnakes() {
	static int moveCounter = 0;
	static int directionChangeCounter = 0;
	for (int i = 0; i < snakes.size(); i++) {
		int posX = snakes[i].getBody()[0].pos.x;
		int posY = snakes[i].getBody()[0].pos.y;
		vector<SnakeBody> &body = snakes[i].getBody();
		int direction = snakes[i].getDirect();

		if (moveCounter >= SNAKE_MOVES * OBJECT_SIZE) {
			for (int j = 0; j < 1; j++) {
				direction = rand() % 4;
				switch (direction) {
				case 0:
					if (snakes[i].getDirect() == 1 || posY - SNAKE_STEP <= 0) {
						j--;
					}
					break;
				case 1:
					if (snakes[i].getDirect() == 0 || posY + SNAKE_STEP >= MAP_SIZEY) {
						j--;
					}
					break;
				case 2:
					if (snakes[i].getDirect() == 3 || posX - SNAKE_STEP <= 0) {
						j--;
					}
					break;
				case 3:
					if (snakes[i].getDirect() == 2 || posX + SNAKE_STEP >= MAP_SIZEX) {
						j--;
					}
					break;
				}
			}
			if (direction != snakes[i].getDirect()) {
				snakes[i].getBody().insert(snakes[i].getBody().begin() + 1, SnakeBody(direction, snakes[i].getBody()[0].bodyDirect2, body[0].pos));
				Vector2f pos = snakes[i].getBody()[0].pos;
				switch (direction)
				{
				case 0: pos.y -= 15; break;
				case 1: pos.y += 15; break;
				case 2: pos.x -= 15; break;
				case 3: pos.x += 15; break;
				}
				snakes[i].getBody()[0].pos = pos;

			}
		}
		if (body.size() >= 3 &&
			(
				(body[body.size() - 1].bodyDirect1 <= 1 && abs(body[body.size() - 1].pos.y - body[body.size() - 2].pos.y) == 20) ||
				(body[body.size() - 1].bodyDirect1 > 1 && abs(body[body.size() - 1].pos.x - body[body.size() - 2].pos.x) == 20))
			) {
			body[body.size() - 1].bodyDirect1 = body[body.size() - 2].bodyDirect1;
			body[body.size() - 1].bodyDirect2 = body[body.size() - 2].bodyDirect1;
			body[body.size() - 1].pos = body[body.size() - 2].pos;
			switch (body[body.size() - 1].bodyDirect1) {
				case 0: body[body.size() - 1].pos.y += 5; break;
				case 1: body[body.size() - 1].pos.y -= 5; break;
				case 2: body[body.size() - 1].pos.x += 5; break;
				case 3: body[body.size() - 1].pos.x -= 5; break;
			}
			body.erase(body.end() - 2);
		}

		switch (direction) {
			case 0: body[0].pos.y -= SNAKE_STEP; break;
			case 1: body[0].pos.y += SNAKE_STEP; break;
			case 2: body[0].pos.x -= SNAKE_STEP; break;
			case 3: body[0].pos.x += SNAKE_STEP; break;
		}
		
		int tailIndex = body.size() - 1;
			switch (body[tailIndex].bodyDirect1) {
			case 0: body[tailIndex].pos.y -= SNAKE_STEP; break;
			case 1: body[tailIndex].pos.y += SNAKE_STEP; break;
			case 2: body[tailIndex].pos.x -= SNAKE_STEP; break;
			case 3: body[tailIndex].pos.x += SNAKE_STEP; break;
			}
		
		snakes[i].setDirect(direction);
		body[0].bodyDirect1 = direction;
		body[0].bodyDirect2 = direction;

	}
	if (directionChangeCounter == OBJECT_SIZE*2) directionChangeCounter = 0;
	else directionChangeCounter += SNAKE_STEP;

	if (moveCounter >= SNAKE_MOVES * OBJECT_SIZE) moveCounter = 0;
	else moveCounter += SNAKE_STEP;
}

void drawSnakes(RenderWindow* window) {
	for (int i = 0; i < snakes.size(); i++) {
		for (int j = 0; j < snakes[i].getSize()-1; j++) {
			int direct = snakes[i].getBody()[j].bodyDirect2;
			switch (direct) {
			case 0:
				SnakeBodySprite->setRotation(degrees(0));
				break;
			case 1:
				SnakeBodySprite->setRotation(degrees(180));
				break;
			case 2:
				SnakeBodySprite->setRotation(degrees(90));
				break;
			case 3:
				SnakeBodySprite->setRotation(degrees(270));
				break;
			}

			int count = 0;
			if (direct == 0 || direct == 1) {
				float spaceY = abs(abs(snakes[i].getBody()[j].pos.y) - abs(snakes[i].getBody()[j+1].pos.y));
				count = round(spaceY / 3.f);
			}
			else {
				float spaceX = abs(abs(snakes[i].getBody()[j].pos.x) - abs(snakes[i].getBody()[j+1].pos.x));
				count = round(spaceX / 3.f);
			}

			for (int k = 5; k < count - 3; k++) {
				switch (direct)
				{
				case 0:
					SnakeBodySprite->setPosition({ snakes[i].getBody()[j].pos.x, snakes[i].getBody()[j].pos.y + k * 3 });
					break;
				case 1:
					SnakeBodySprite->setPosition({ snakes[i].getBody()[j].pos.x, snakes[i].getBody()[j].pos.y - k * 3 });
					break;
				case 2:
					SnakeBodySprite->setPosition({ snakes[i].getBody()[j].pos.x + k * 3, snakes[i].getBody()[j].pos.y });
					break;
				case 3:
					SnakeBodySprite->setPosition({ snakes[i].getBody()[j].pos.x - k * 3, snakes[i].getBody()[j].pos.y });
					break;
				}
				window->draw(*SnakeBodySprite);
			}
		}
		for (int j = 1; j < snakes[i].getSize() - 1; j++) {
			SnakeBodyBendSprite->setPosition(snakes[i].getBody()[j].pos);
			int direct2 = snakes[i].getBody()[j + 1].bodyDirect1;
			int direct3 = snakes[i].getBody()[j - 1].bodyDirect2;
			if (((direct2 == 3 && direct3 == 0) || (direct2 == 1 && direct3 == 2))) {
				SnakeBodyBendSprite->setRotation(degrees(0));
			}
			else if ((direct2 == 2 && direct3 == 0) || (direct2 == 1 && direct3 == 3)) {
				SnakeBodyBendSprite->setRotation(degrees(90));
			}
			else if ((direct2 == 0 && direct3 == 3) || (direct2 == 2 && direct3 == 1)) {
				SnakeBodyBendSprite->setRotation(degrees(180));
			}
			else if ((direct2 == 3 && direct3 == 1) || (direct2 == 0 && direct3 == 2)) {
				SnakeBodyBendSprite->setRotation(degrees(270));
			}
			window->draw(*SnakeBodyBendSprite);
		}
		Angle tailAngle;
		if (snakes[i].getSize() - 1 >= 0) {
			switch (snakes[i].getBody()[snakes[i].getSize() - 1].bodyDirect1) {
			case 0: tailAngle = degrees(0); break;
			case 1: tailAngle = degrees(180); break;
			case 2: tailAngle = degrees(270); break;
			case 3: tailAngle = degrees(90); break;
			}
			SnakeTailSprite->setRotation(tailAngle);
			SnakeTailSprite->setPosition(snakes[i].getBody()[snakes[i].getSize() - 1].pos);
			window->draw(*SnakeTailSprite);
		}
		Angle headAngle;
		switch (snakes[i].getBody()[0].bodyDirect1) {
		case 0: headAngle = degrees(0); break;
		case 1: headAngle = degrees(180); break;
		case 2: headAngle = degrees(270); break;
		case 3: headAngle = degrees(90); break;
		}
		SnakeHeadSprite->setRotation(headAngle);
		SnakeHeadSprite->setPosition(snakes[i].getBody()[0].pos);
		window->draw(*SnakeHeadSprite);
	}
}

bool inRange(int value, int a, int b) {
	return value >= min(a, b) && value <= max(a, b);
}

void attackSnake(int playerDirect) {
	for (int i = 0; i < snakes.size(); i++) {
		for (int j = 0; j < snakes[i].getBody().size() - 1; j++) {
			Vector2f snakePos1 = snakes[i].getBody()[j].pos;
			Vector2f snakePos2 = snakes[i].getBody()[j+1].pos;
			float x1 = snakePos1.x < snakePos2.x ? snakePos1.x : snakePos2.x;
			float x2 = snakePos1.x > snakePos2.x ? snakePos1.x : snakePos2.x;
			float y1 = snakePos1.y < snakePos2.y ? snakePos1.y : snakePos2.y;
			float y2 = snakePos1.y > snakePos2.y ? snakePos1.y : snakePos2.y;
			if ((snakePos1.y == snakePos2.y) && inRange(playerPosX + PLAYER_SIZEX / 2, x1, x2) &&
				inRange(playerPosY + PLAYER_SIZEY / 2, y1 - OBJECT_SIZE * 2, y2 + OBJECT_SIZE * 2)) {
				hittedSnakes[i]++;
				snakes[i].hitSnake();
				score += 10;
				PlayHitSnakeSound();
			}
			else if ((snakePos1.x == snakePos2.x) && inRange(playerPosX + PLAYER_SIZEX / 2, x1 - OBJECT_SIZE * 2, x2 + OBJECT_SIZE * 2) &&
				inRange(playerPosY + PLAYER_SIZEY / 2, y1, y2)) {
				hittedSnakes[i]++;
				snakes[i].hitSnake();
				score += 10;
				PlayHitSnakeSound();
			}
		}
	}
}

void deleteSnakes() {
	for (size_t i = 0; i < snakes.size();) {
		if (snakes[i].getLength() <= 3) {
			score += 30;
			snakes.erase(snakes.begin() + i);
		}
		else {
			++i;
		}
	}
}


void drawGround(RenderWindow* window) {
	for (float posX = 0; posX < MAP_SIZEX; posX += 100) {
		for (float posY = 0; posY < MAP_SIZEY; posY += 100) {
			GroundSprite->setPosition({ posX, posY });
			window->draw(*GroundSprite);
		}
	}
}

bool isObjectPosNew(float posX, float posY) {
	for (int i = 0; i < objects.size(); i++) {
		if (posX == objects[i].getPos().x && posY == objects[i].getPos().y) {
			return false;
		}
	}
	return true;
}
void snakeBite() {
	float posX;
	float posY;
	for (int i = 0; i < snakes.size(); i++) {
		posX = snakes[i].getBody()[0].pos.x;
		posY = snakes[i].getBody()[0].pos.y;
		if (inRange(playerPosX + PLAYER_SIZEX / 2, posX - OBJECT_SIZE, posX + OBJECT_SIZE * 3) &&
			inRange(playerPosY + PLAYER_SIZEY / 2, posY - OBJECT_SIZE, posY + OBJECT_SIZE * 2)) {
			isPoisoned = true;
			poisonClock->restart();
			PlaySnakeBiteSound();
			PlayTimerSound();
		}
	}
}
void setAntidotesPos(int maxCount) {
	float posX;
	float posY;
	int count = MIN_NUM_OF_ANTIDOTES + rand() % (maxCount - MIN_NUM_OF_ANTIDOTES + 1);
	for (int i = 0; i < count; i++) {
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;
		if (isObjectPosNew(posX, posY)) {
			antidotes.push_back({ posX + OBJECT_SIZE / 2, posY + OBJECT_SIZE / 2 });
		}
		else
			i--;
	}
}
void drawAntidotes(RenderWindow* window) {
	for (int i = 0; i < antidotes.size(); i++) {
		AntidoteSprite->setPosition(antidotes[i]);
		window->draw(*AntidoteSprite);
	}

}
void useAntidote() {
	for (int i = 0; i < antidotes.size(); i++) {
		if (inRange(playerPosX + PLAYER_SIZEX / 2, antidotes[i].x - OBJECT_SIZE / 2, antidotes[i].x + OBJECT_SIZE * 2) &&
			inRange(playerPosY + PLAYER_SIZEY / 2, antidotes[i].y - OBJECT_SIZE / 2, antidotes[i].y + OBJECT_SIZE)) {
			PlayAntidoteSound();
			isPoisoned = false;
			poisonTimer = 0;
			antidotes.erase(antidotes.begin() + i);
			usedAntidotes.push_back({ antidotes[i].x, antidotes[i].y });
			return;
		}
	}
}

void displayScore(RenderWindow* window) {
	textScore->setPosition({ viewPosX - SCREEN_RESX / 2 + 30 , viewPosY - SCREEN_RESY / 2 + 50 });
	textScore->setString("SCORE: " + to_string(score));
	textBestScore->setPosition({ viewPosX - SCREEN_RESX / 2 + 30 , viewPosY - SCREEN_RESY / 2 + 15 });
	textBestScore->setString("BEST SCORE: " + to_string(bestScore));
	window->draw(*textScore);
	window->draw(*textBestScore);
}

void displayTimeToDeath(RenderWindow* window) {
	if (isPoisoned) {
		timeToDeath->setOrigin({ timeToDeath->getLocalBounds().getCenter().x, 0 });
		SkullSprite->setPosition({ viewPosX - SCREEN_RESX / 2 + 30 + SkullTexture.getSize().x / 2, viewPosY - SCREEN_RESY / 2 + 135 });
		timeToDeath->setPosition({ viewPosX - SCREEN_RESX / 2 + 30 + SkullTexture.getSize().x / 2,
								   viewPosY - SCREEN_RESY / 2 + SkullTexture.getSize().y + 100 });

		float timeLeft = DEATH - poisonTimer;
		if (timeLeft <= 6.f) {
			PlayTimerSound(1.7f);
		}
		else {
			PlayTimerSound(1.0f);
		}
		//DEATH / 2 + 1
		timeToDeath->setFillColor(timeLeft <= 6 ? Color::Red : Color::Black);
		timeToDeath->setString(to_string((int)(DEATH - poisonTimer)));
		window->draw(*timeToDeath);
		window->draw(*SkullSprite);
	}
}

void drawPlayers(RenderWindow* window) {
	static std::unordered_map<int, Player> drawPos;
	for (auto& player : players) {
		drawPos[player.first].pos += (player.second.pos - drawPos[player.first].pos) * 0.1f;
		PlayerSprite->setPosition(drawPos[player.first].pos);
		PlayerSprite->setTexture(PlayerFrontTexture, false);
		switch (player.second.direction) {
		case 0:
			PlayerSprite->setTexture(PlayerBackTexture, false); break;
		case 1:
			PlayerSprite->setTexture(PlayerFrontTexture, false); break;
		case 2:
			PlayerSprite->setTexture(PlayerLeftTexture, false); break;
		case 3:
			PlayerSprite->setTexture(PlayerRightTexture, false); break;
		}
		window->draw(*PlayerSprite);

		Text textId(font, "Player " + to_string(player.first), 20);
		textId.setOrigin(textId.getLocalBounds().getCenter());
		textId.setPosition({ drawPos[player.first].pos.x, drawPos[player.first].pos.y - 70 });
		window->draw(textId);
	}
}

void restart() {
	isTheBest();
	for (int i = 0; i < snakes.size(); i++) {
		snakes[i].getBody().clear();
	}
	snakes.clear();
	objects.clear();
	antidotes.clear();
	score = 0;
	isPoisoned = false;
	poisonTimer = 0;
	playerPosX = (MAP_SIZEX / 2);
	playerPosY = (MAP_SIZEY / 2);
	playerDirection = 1;
	viewPosX = (MAP_SIZEX / 2);
	viewPosY = (MAP_SIZEY / 2);
	setObjectsPos();
	setAntidotesPos(MAX_NUM_OF_ANTIDOTES);
	spawnSnakes(NUM_OF_SNAKES);
}

void isTheBest() {
	if (score > bestScore) {
		bestScore = score;
	}
}

int InputConnectBox(RenderWindow* window, Text* IP, Text* code, int *inputType) {
	float posForX = viewPosX - SCREEN_RESX / 2;
	float posForY = viewPosY - SCREEN_RESY / 2;
	float posForIPFieldX = viewPosX - 150;
	float posForIPFieldY = viewPosY - 5;
	float posForCodeFieldX = viewPosX - 90;
	float posForCodeFieldY = viewPosY + 95;
	float posForButtonCreateX = viewPosX - 170;
	float posForButtonCreateY = viewPosY + 160;
	float posForButtonConnectX = viewPosX + 20;
	float posForButtonConnectY = viewPosY + 160;
	float posForButtonCancelX = viewPosX - 75;
	float posForButtonCancelY = viewPosY + 230;

	static Clock clickClock;

	int mode = 0;

	window->draw(*LogoSprite);

	Text *inputIP = new Text(font, "Server IP:");
	inputIP->setCharacterSize(30);
	inputIP->setFillColor(Color::Black);
	inputIP->setPosition({ posForIPFieldX, posForIPFieldY - 40 });
	window->draw(*inputIP);

	Text* inputCode = new Text(font, "Invite Code:");
	inputCode->setCharacterSize(30);
	inputCode->setFillColor(Color::Black);
	inputCode->setPosition({ posForIPFieldX, posForCodeFieldY - 40 });
	window->draw(*inputCode);

	sf::RectangleShape IPField({ 300.f, 50.f });
	IPField.setPosition({ posForIPFieldX, posForIPFieldY });
	window->draw(IPField);

	sf::RectangleShape codeField({ 180.f, 50.f });
	codeField.setPosition({ posForCodeFieldX, posForCodeFieldY	});
	window->draw(codeField);

	sf::RectangleShape buttonCreate({ 150.f, 50.f });
	buttonCreate.setPosition({ posForButtonCreateX, posForButtonCreateY });
	window->draw(buttonCreate);

	Text createText(font, "Create", 30);
	createText.setFillColor(Color::Black);
	createText.setOrigin({ createText.getLocalBounds().getCenter().x, createText.getLocalBounds().getCenter().y });
	createText.setPosition({ posForButtonCreateX + 75, posForButtonCreateY + 25 });
	window->draw(createText);

	sf::RectangleShape buttonConnect({ 150.f, 50.f });
	buttonConnect.setPosition({ posForButtonConnectX, posForButtonConnectY });
	window->draw(buttonConnect);

	Text connectText(font, "Connect", 30);
	connectText.setFillColor(Color::Black);
	connectText.setOrigin({ connectText.getLocalBounds().getCenter().x, connectText.getLocalBounds().getCenter().y });
	connectText.setPosition({ posForButtonConnectX + 75, posForButtonConnectY + 25 });
	window->draw(connectText);

	sf::RectangleShape buttonCancel({ 150.f, 50.f });
	buttonCancel.setPosition({ posForButtonCancelX, posForButtonCancelY });
	window->draw(buttonCancel);

	Text cancelText(font, "Cancel", 30);
	cancelText.setFillColor(Color::Red);
	cancelText.setOrigin({ cancelText.getLocalBounds().getCenter().x, cancelText.getLocalBounds().getCenter().y });
	cancelText.setPosition({ posForButtonCancelX + 75, posForButtonCancelY + 25 });
	window->draw(cancelText);

	IP->setPosition({ posForIPFieldX, posForIPFieldY });
	window->draw(*IP);
	code->setPosition({ posForCodeFieldX, posForCodeFieldY });
	window->draw(*code);

	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		Vector2i mousePos = Mouse::getPosition(*window);

		if (inRange(posForX + mousePos.x, posForIPFieldX, posForIPFieldX + 300) &&
			inRange(posForY + mousePos.y, posForIPFieldY, posForIPFieldY + 50)) {
			mtx2.lock();
			*inputType = 1;
			mtx2.unlock();
		}
		else if (inRange(posForX + mousePos.x, posForCodeFieldX, posForCodeFieldX + 150) &&
			inRange(posForY + mousePos.y, posForCodeFieldY, posForCodeFieldY + 50)) {
			mtx2.lock();
			*inputType = 2;
			mtx2.unlock();
		}


		if (clickClock.getElapsedTime().asMilliseconds() > 300) {
			if (inRange(posForX + mousePos.x, posForButtonCreateX, posForButtonCreateX + 150) &&
				inRange(posForY + mousePos.y, posForButtonCreateY, posForButtonCreateY + 50)) {
				mode = 1;
				clickClock.restart();
			}
			else if (inRange(posForX + mousePos.x, posForButtonConnectX, posForButtonConnectX + 150) &&
				inRange(posForY + mousePos.y, posForButtonConnectY, posForButtonConnectY + 50)) {
				mode = 2;
				clickClock.restart();
			}
			else if (inRange(posForX + mousePos.x, posForButtonCancelX, posForButtonCancelX + 150) &&
				inRange(posForY + mousePos.y, posForButtonCancelY, posForButtonCancelY + 50)) {
				restart();
				mtx_game_status.lock();
				game_status = 0;
				mtx_game_status.unlock();
				pauseTimer = 0;
				clickClock.restart();
				return 0;
			}
		}
		bool correctIP = IP->getString().getSize() >= 12;
		bool correctCode = code->getString().getSize() >= 8;
		if (mode == 1 && correctIP) {
			return 1;
		}
		if (mode == 2 && correctIP && correctCode) {
			return 2;
		}
		return 0;
	}
}

void processInputEvent(optional<Event>* event, Text* drawableText, string * str) {
	if (auto* textEntered = (*event)->getIf<sf::Event::TextEntered>())
	{
		auto c = textEntered->unicode;
		if ((c >= '0' && c <= '9') || c == '.')
		{
			mtx1.lock();
			str->push_back(static_cast<char>(c));
			mtx1.unlock();
		}
	}
	if (auto* keyPressed = (*event)->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::Backspace && !str->empty())
		{
			mtx1.lock();
			str->pop_back();
			mtx1.unlock();
		}
	}
	mtx1.lock();
	drawableText->setString(*str);
	mtx1.unlock();
}