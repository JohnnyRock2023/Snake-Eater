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
				// continue;
			}
		}
		objects.push_back(Object(type, posX, posY));
	}
}

void fillTheMapWithObj(RenderWindow* window) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].getType() == 0) {
			GrassShape.setPosition(objects[i].getPos());
			window->draw(GrassShape);
		}
		else if (objects[i].getType() == 1) {
			RockShape.setPosition(objects[i].getPos());
			window->draw(RockShape);
		}
		else if (objects[i].getType() == 2) {
			StumpShape.setPosition(objects[i].getPos());
			window->draw(StumpShape);
		}
		else if (objects[i].getType() == 3) {
			BushShape.setPosition(objects[i].getPos());
			window->draw(BushShape);
		}
	}
}

bool isPosNew(Vector2f pos) {
	float posX;
	float posY;
	for (int i = 0; i < snakes.size(); i++) {
		posX = snakes[i].getBody()[0].x;
		posY = snakes[i].getBody()[0].y;
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

		vector<Vector2f> body;
		body.push_back({ posX, posY });

		for (int k = 1; k < size; k++) {
			switch (direction) {
			case 0: body.push_back({ posX, posY + k * OBJECT_SIZE }); break;
			case 1: body.push_back({ posX, posY - k * OBJECT_SIZE }); break;
			case 2: body.push_back({ posX + k * OBJECT_SIZE, posY }); break;
			case 3: body.push_back({ posX - k * OBJECT_SIZE, posY }); break;
			}
		}
		snakes.push_back(Snake(size, direction, body));
	}
}

//for (int j = 0; j < snakes.size(); j++) {
/*if (posX == snakes[j + 1].getBody()[snakes[j + 1].getSize() - 1].x && posY == snakes[j + 1].getBody()[snakes[j + 1].getSize() - 1].y) {
		i--;
		break;
}*/


/*case 0:
	snakes[pos].getBody().push_back({ snakes[pos].getBody()[k].x, snakes[pos].getBody()[k].y - OBJECT_SIZE });
	break;
case 1:
	snakes[pos].getBody().push_back({ snakes[pos].getBody()[k].x, snakes[pos].getBody()[k].y + OBJECT_SIZE });
	break;
case 2:
	snakes[pos].getBody().push_back({ snakes[pos].getBody()[k].x - OBJECT_SIZE, snakes[pos].getBody()[k].y });
	break;
case 3:
	snakes[pos].getBody().push_back({ snakes[pos].getBody()[k].x + OBJECT_SIZE, snakes[pos].getBody()[k].y });
	break;
}*/

void moveSnakes() {
	static int moves = 0;
	int posX;
	int posY;
	int direction;
	for (int i = 0; i < snakes.size(); i++) {

		posX = snakes[i].getBody()[0].x;
		posY = snakes[i].getBody()[0].y;

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
					if (snakes[i].getDirect() == 0 || snakes[i].getDirect() == 1 || posY + OBJECT_SIZE >= MAP_SIZEY) {
						j--;
					}
					break;
				case 2:
					if (snakes[i].getDirect() == 3 || snakes[i].getDirect() == 2 || posX - OBJECT_SIZE < 0) {
						j--;
					}
					break;
				case 3:
					if (snakes[i].getDirect() == 2 || snakes[i].getDirect() == 3 || posX + OBJECT_SIZE >= MAP_SIZEX) {
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
				if (snakes[i].getDirect() == 0 || posY + OBJECT_SIZE >= MAP_SIZEY) {
					j--;
				}
				break;
			case 2:
				if (snakes[i].getDirect() == 3 || posX - OBJECT_SIZE < 0) {
					j--;
				}
				break;
			case 3:
				if (snakes[i].getDirect() == 2 || posX + OBJECT_SIZE >= MAP_SIZEX) {
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
		}
		switch (direction) {
		case 0:
			snakes[i].getBody()[0].y -= OBJECT_SIZE;
			break;
		case 1:
			snakes[i].getBody()[0].y += OBJECT_SIZE;
			break;
		case 2:
			snakes[i].getBody()[0].x -= OBJECT_SIZE;
			break;
		case 3:
			snakes[i].getBody()[0].x += OBJECT_SIZE;
			break;
		}
		snakes[i].setDirect(direction);
	}
	if (moves == 0) {
		moves = SNAKE_MOVES;
	}
	else {
		moves--;
	}
}

void drawSnakes(RenderWindow* window) {
	for (int i = 0; i < snakes.size(); i++) {
		SnakeHeadShape.setPosition(snakes[i].getBody()[0]);
		window->draw(SnakeHeadShape);
		for (int j = 1; j < snakes[i].getBody().size(); j++) {
			SnakeBodyShape.setPosition(snakes[i].getBody()[j]);
			window->draw(SnakeBodyShape);
		}
	}
}