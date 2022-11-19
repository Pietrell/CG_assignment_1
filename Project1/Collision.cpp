#include "Collision.h"

void updateGlobalCoordinates(Figura* fig) {
	vec4 t = fig->Model * vec4(fig->box.corner_b, 1.0);
	fig->box.corner_b_glob.x = t.x;
	fig->box.corner_b_glob.y = t.y;
	fig->box.corner_b_glob.z = t.z;

	t = fig->Model * vec4(fig->box.corner_t, 1.0);
	fig->box.corner_t_glob.x = t.x;
	fig->box.corner_t_glob.y = t.y;
	fig->box.corner_t_glob.z = t.z;

}


bool checkCollision(Figura* fig1, Figura* fig2) {
	updateGlobalCoordinates(fig1);
	updateGlobalCoordinates(fig2);
	//collisioni su asse x
	bool collisionX = fig1->box.corner_b_glob.x <= fig2->box.corner_t_glob.x && fig1->box.corner_t_glob.x >= fig2->box.corner_b_glob.x;
	//collisioni su asse y
	bool collisionY = fig1->box.corner_b_glob.y <= fig2->box.corner_t_glob.y && fig1->box.corner_t_glob.y >= fig2->box.corner_b_glob.y;
	return collisionX && collisionY;
}

void calculateBoundingBox(Figura* fig) {
	float ymin, ymax, xmin, xmax;
	ymin = ymax = xmin = xmax = 0.0; //inizializzo tutte le vatriabili a 0
	for (int i = 0; i < fig->vertex.size(); i++)
	{
		if (fig->vertex[i].x > xmax) {
			xmax = fig->vertex[i].x;
		}
		if (fig->vertex[i].x < xmin) {
			xmin = fig->vertex[i].x;
		}
		if (fig->vertex[i].x > xmax) {
			ymax = fig->vertex[i].y;
		}
		if (fig->vertex[i].x < xmin) {
			ymin = fig->vertex[i].y;
		}
	}
	fig->box.corner_t = vec3(xmax,ymax,0.0);
	fig->box.corner_b = vec3(xmin,ymin,0.0);

}
