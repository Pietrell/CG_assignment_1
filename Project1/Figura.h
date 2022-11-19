#pragma once
#include <iostream>
#include "Lib.h"



typedef struct {
	vec3 corner_b_glob;
	vec3 corner_t_glob;
	vec3 corner_b;
	vec3 corner_t;
}BoundingBox;

typedef struct {
	GLuint VAO;
	GLuint VBO_Vert;
	GLuint VBO_Col;
	GLuint EBO_indici;

	// Vertici	in valori locali
	vector<vec3> vertex;
	vector<vec4> colorVertex;
	vector<vec3> Hermite;
	vector<vec4> colorHermite;
	vector<int> index;
	vector<vec2> boudLimit;

	vec3 globalPos;
	// Numero vertici
	int nv;
	int nvHer;
	bool hasHerm,draw, isProjectile;
	BoundingBox box;

	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	float AngoloRotazione;
	unsigned int sceltaFS;
} Figura;
