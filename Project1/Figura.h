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

	// Vertici	in valori locali
	vector<vec3> vertex;
	vector<vec4> colorVertex;
	vector<vec3> Hermite;
	vector<vec4> colorHermite;
	vector<int> index;

	vec3 globalPos;
	// Numero vertici
	int nv;
	int nvHer;
	bool hasHerm,draw;
	BoundingBox box;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	float AngoloRotazione; // rispetto asse x
	unsigned int sceltaFS;
} Figura;
