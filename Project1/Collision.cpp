#include "Collision.h"

bool checkCollision(BoundingBox obj1, BoundingBox obj2) {
	//collisioni su asse x
	bool collisionX = obj1.corner_b.x <= obj2.corner_t.x && obj1.corner_t.x >= obj2.corner_b.x;
	//collisioni su asse y
	bool collisionY = obj1.corner_b.y <= obj2.corner_t.y &&	obj1.corner_t.y >= obj2.corner_b.y;
	return collisionX && collisionY;
}

void calculateBoundingBox(Figura* fig) {


}
void updateModel(mat4 matModel) {


}