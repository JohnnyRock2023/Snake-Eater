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
			if (posX == objects[j].pos.x && posY == objects[j].pos.y) {
				i--;
				continue;
			}
		}
		objects.push_back(Object(type, posX, posY));
	}
}

void fillTheMapWithObj(RenderWindow *window) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].type == 0) {
			GrassShape.setPosition(objects[i].pos);
			window->draw(GrassShape);
		}
		else if (objects[i].type == 1) {
			RockShape.setPosition(objects[i].pos);
			window->draw(RockShape);
		}
		else if (objects[i].type == 2) {
			StumpShape.setPosition(objects[i].pos);
			window->draw(StumpShape);
		}
		else if (objects[i].type == 3) {
			BushShape.setPosition(objects[i].pos);
			window->draw(BushShape);
		}
	}
}