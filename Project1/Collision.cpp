#include "Collision.h"

bool checkCollision(BoundingBox obj1, BoundingBox obj2) {
	// guardo collisioni su asse x
	bool collisionX = obj1.corner_b.x <= obj2.corner_t.x &&
		obj1.corner_t.x >= obj2.corner_b.x;

	// guardo collisioni su asse y
	bool collisionY = obj1.corner_b.y <= obj2.corner_t.y &&
		obj1.corner_t.y >= obj2.corner_b.y;
	//Si ha collisione se c'� collisione sia nella direzione x che nella direzione y

	return collisionX && collisionY;

}


void addVertices(vector<vec3> vertici)
{
	float xmin, xmax, ymin, ymax;
	xmin = vertici[0].x;
	xmax = vertici[0].x;
	ymax = vertici[0].y;
	ymin = vertici[0].y;
	for (int i = 1; i < vertici.size(); i++)
	{
		// cout << vertici[i].x << " " << vertici[i].y << endl;
		if (vertici[i].x < xmin)
			xmin = vertici[i].x;
		if (vertici[i].x > xmax)
			xmax = vertici[i].x;
		if (vertici[i].y < ymin)
			ymin = vertici[i].y;
		if (vertici[i].y > ymax)
			ymax = vertici[i].y;
	}
	//bot = vec4(xmin, ymin, 0.0, 1.0);
	//top = vec4(xmax, ymax, 0.0, 1.0);
}
void calculateBoundingBox(Figura* fig) {


}
void updateModel(mat4 matModel) {


}