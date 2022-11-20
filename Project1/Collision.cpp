#include "Collision.h"

void updateGlobalCollisionCoordinates(Figura* fig) {
	vector<vec4 >t;
	t.push_back(vec4(fig->Model * vec4(fig->box.corner_b, 1.0)));	
	t.push_back(vec4(fig->Model * vec4(fig->box.corner_t, 1.0)));
	

	float ymin, ymax, xmin, xmax;
	ymin = ymax = xmin = xmax = 0.0; //inizializzo tutte le vatriabili a 0
	if (t[0].x < t[1].x) {
		xmin = t[0].x;
		xmax = t[1].x;
	}
	else {
		xmin = t[1].x;
		xmax = t[0].x;
	}


	if (t[0].y < t[1].y) {
		ymin = t[0].y;
		ymax = t[1].y;
	}
	else {
		ymin = t[1].y;
		ymax = t[0].y;
	}




	fig->box.corner_t_glob = vec3(xmax, ymax, 0.0);
	fig->box.corner_b_glob = vec3(xmin, ymin, 0.0);

}


bool checkCollision(Figura* fig1, Figura* fig2) {
	/*updateGlobalCollisionCoordinates(fig1);
	updateGlobalCollisionCoordinates(fig2);*/
	//collisioni su asse x
	bool collisionX = fig1->box.corner_b_glob.x <= fig2->box.corner_t_glob.x && fig1->box.corner_t_glob.x >= fig2->box.corner_b_glob.x;
	//collisioni su asse y
	bool collisionY = fig1->box.corner_b_glob.y <= fig2->box.corner_t_glob.y &&	fig1->box.corner_t_glob.y >= fig2->box.corner_b_glob.y;
	return (collisionX && collisionY);
}

void calculateBoundingBox(Figura* fig) { // calcolo bounding box in coordinate locali dell'oggetto
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
		if (fig->vertex[i].y > ymax) {
			ymax = fig->vertex[i].y;
		}
		if (fig->vertex[i].y < ymin) {
			ymin = fig->vertex[i].y;
		}
	}
	fig->box.corner_t = vec3(xmax,ymax,0.0);
	fig->box.corner_b = vec3(xmin,ymin,0.0);

}
