#include "Header.h"

void setObjectsPos() {
	float posX;
	float posY;
	int type;
	for (int i = 0; i < (MIN_NUM_OF_OBJECTS + rand() % MAX_NUM_OF_OBJECTS); i++) {
		type = rand() % 4;
		posX = (float)(rand() % (MAP_SIZEX / OBJECT_SIZE)) * OBJECT_SIZE;
		posY = (float)(rand() % (MAP_SIZEY / OBJECT_SIZE)) * OBJECT_SIZE;
		
		for (int j = 0; j < objects.size(); j++) {
			if (posX == objects[j].getPos().x && posY == objects[j].getPos().y) {
				i--;
				continue;
			}
		}
		objects.push_back(Object(type, posX, posY));
	}
}

void fillTheMapWithObj(RenderWindow *window) {
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