#include "Header.h"

void setObjectsPos() {
	float posX;
	float posY;
	short int type;
	for (int i = 0; i < (MIN_NUM_OF_OBJECTS + rand() % MAX_NUM_OF_OBJECTS); i++) {
		type = rand() % 4;
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;

		for (int j = 0; j < objects.size(); j++) {
			if (posX == objects[j].getPos().x && posY == objects[j].getPos().y) {
				i--;
			}
		}
		objects.push_back(Object(type, posX, posY));
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
			window->draw(*RockSprite);
		}
		else if (objects[i].getType() == 2) {
			StumpSprite->setPosition(objects[i].getPos());
			window->draw(*StumpSprite);
		}
		else if (objects[i].getType() == 3) {
			BushSprite->setPosition(objects[i].getPos());
			window->draw(*BushSprite);
		}
	}
}

bool isPosNew(Vector2f pos) {
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

void spawnSnakes(int count) {
	float posX;
	float posY;
	short int direction;
	short int size;
	for (int i = 0; i < count; i++) {
		direction = rand() % 4;
		size = MIN_SNAKE_SIZE + rand() % (MAX_SNAKE_SIZE - MIN_SNAKE_SIZE + 1);
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;

		while (!isPosNew({ posX, posY })) {
			posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
			posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;
		}

		vector<SnakeBody> body;
		body.push_back(SnakeBody(direction, { posX, posY }));

		for (int k = 1; k < size; k++) {
			switch (direction) {
			case 0: body.push_back(SnakeBody(direction, { posX, posY + k * OBJECT_SIZE })); break;
			case 1: body.push_back(SnakeBody(direction, { posX, posY - k * OBJECT_SIZE })); break;
			case 2: body.push_back(SnakeBody(direction, { posX + k * OBJECT_SIZE, posY })); break;
			case 3: body.push_back(SnakeBody(direction, { posX - k * OBJECT_SIZE, posY })); break;
			}
		}
		snakes.push_back(Snake(size, direction, body));
	}
}

void moveSnakes() {
	static int moves = 0;
	int posX;
	int posY;
	int direction;
	for (int i = 0; i < snakes.size(); i++) {

		posX = snakes[i].getBody()[0].pos.x;
		posY = snakes[i].getBody()[0].pos.y;

		for (int j = 0; j < 1; j++) {
			if (moves == 0) {
				direction = rand() % 4;
				switch (direction) {
					// Ver 1
						/*case 0:
							if (snakes[i].getDirect() == 1 || snakes[i].getDirect() == 0 || posY - OBJECT_SIZE < 0) {
								j--;
							}
							break;
						case 1:
							if (snakes[i].getDirect() == 0 || snakes[i].getDirect() == 1 || posY + OBJECT_SIZE > MAP_SIZEY) {
								j--;
							}
							break;
						case 2:
							if (snakes[i].getDirect() == 3 || snakes[i].getDirect() == 2 || posX - OBJECT_SIZE < 0) {
								j--;
							}
							break;
						case 3:
							if (snakes[i].getDirect() == 2 || snakes[i].getDirect() == 3 || posX + OBJECT_SIZE > MAP_SIZEX) {
								j--;
							}
							break;
						}*/
						// Ver 2
				case 0:
					if (snakes[i].getDirect() == 1 || posY - OBJECT_SIZE < 0) {
						j--;
					}
					break;
				case 1:
					if (snakes[i].getDirect() == 0 || posY + OBJECT_SIZE > MAP_SIZEY) {
						j--;
					}
					break;
				case 2:
					if (snakes[i].getDirect() == 3 || posX - OBJECT_SIZE < 0) {
						j--;
					}
					break;
				case 3:
					if (snakes[i].getDirect() == 2 || posX + OBJECT_SIZE > MAP_SIZEX) {
						j--;
					}
					break;
				}
			}
			else {
				direction = snakes[i].getDirect();
			}
		}
		for (int j = snakes[i].getBody().size() - 1; j >= 1; j--) {
			snakes[i].getBody()[j] = snakes[i].getBody()[j - 1];
			snakes[i].getBody()[j].bodyDirect = snakes[i].getBody()[j - 1].bodyDirect;
		}
		switch (direction) {
		case 0:
			snakes[i].getBody()[0].pos.y -= OBJECT_SIZE;
			break;
		case 1:
			snakes[i].getBody()[0].pos.y += OBJECT_SIZE;
			break;
		case 2:
			snakes[i].getBody()[0].pos.x -= OBJECT_SIZE;
			break;
		case 3:
			snakes[i].getBody()[0].pos.x += OBJECT_SIZE;
			break;
		}
		snakes[i].setDirect(direction);
		snakes[i].getBody()[0].bodyDirect = direction;
	}
	if (moves == 0) {
		moves = SNAKE_MOVES;
	}
	else {
		moves--;
	}
}

void drawSnakes(RenderWindow* window) {
	int direct1;
	int direct2 = -1;
	int direct3 = -1;
	int size;
	for (int i = 0; i < snakes.size(); i++) {
		size = snakes[i].getBody().size();
		Angle headAngle;
		switch (snakes[i].getBody()[0].bodyDirect) {
			case 0: headAngle = degrees(0); break;
			case 1: headAngle = degrees(180); break;
			case 2: headAngle = degrees(90); break;
			case 3: headAngle = degrees(270); break;
		}
		SnakeHeadSprite->setRotation(headAngle);
		SnakeHeadSprite->setPosition(snakes[i].getBody()[0].pos);
		window->draw(*SnakeHeadSprite);
		for (int j = 1; j < size - 1; j++) {
			SnakeBodySprite->setPosition(snakes[i].getBody()[j].pos);
			SnakeBodySprite->setTexture(SnakeBodyBendTexture, false);

			direct1 = snakes[i].getBody()[j].bodyDirect;

			if (j + 1 < snakes[i].getBody().size()) {
				direct2 = snakes[i].getBody()[j + 1].bodyDirect;
				direct3 = snakes[i].getBody()[j - 1].bodyDirect;
			}

			if (((direct2 == 3 && direct3 == 0) || (direct2 == 1 && direct3 == 2)) && direct1 != direct3) {
				SnakeBodySprite->setRotation(degrees(0));
			}
			else if (((direct2 == 1 && direct3 == 3) || (direct2 == 2 && direct3 == 0)) && direct1 != direct3) {
				SnakeBodySprite->setRotation(degrees(90));
			}
			else if (((direct2 == 2 && direct3 == 1) || (direct2 == 0 && direct3 == 3)) && direct1 != direct3) {
				SnakeBodySprite->setRotation(degrees(180));
			}
			else if (((direct2 == 3 && direct3 == 1) || (direct2 == 0 && direct3 == 2)) && direct1 != direct3) {
				SnakeBodySprite->setRotation(degrees(270));
			}
			else if (direct1 == 0 || direct1 == 1) {
				SnakeBodySprite->setTexture(SnakeBodyTexture, false);
				SnakeBodySprite->setRotation(degrees(0));
			}
			else if (direct1 == 2 || direct1 == 3) {
				SnakeBodySprite->setTexture(SnakeBodyTexture, false);
				SnakeBodySprite->setRotation(degrees(90));
			}
			direct2 = -1;
			direct3 = -1;
			window->draw(*SnakeBodySprite);
		}
		Angle tailAngle;
		switch (snakes[i].getBody()[size-2].bodyDirect) {
		case 0: tailAngle = degrees(0); break;
		case 1: tailAngle = degrees(180); break;
		case 2: tailAngle = degrees(270); break;
		case 3: tailAngle = degrees(90); break;
		}
		SnakeTailSprite->setRotation(tailAngle);
		SnakeTailSprite->setPosition(snakes[i].getBody()[size-1].pos);
		window->draw(*SnakeTailSprite);
	}
}

void attackSnake(Vector2f playerPos, int playerDirect) {
	Vector2f snakePos;
	for (int i = 0; i < snakes.size(); i++) {
		for (int j = 0; j < snakes[i].getBody().size(); j++) {
			snakePos = snakes[i].getBody()[j].pos;
			if (playerDirect == 0 && playerPos.x == snakePos.x && playerPos.y == snakePos.y) {
				if (playerPos.x == snakePos.x && (playerPos.y == snakePos.y || playerPos.y - OBJECT_SIZE == snakePos.y)) {

				}

			}
		}
	}
}
