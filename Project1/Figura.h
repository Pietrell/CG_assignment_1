#pragma once
#include <iostream>
#include "Lib.h"

typedef struct {
	GLuint VAO;
	GLuint VBO_Vert;
	GLuint VBO_Col;
	GLuint EBO_indici;

	// Vertici	
	vector<vec3> vertex;
	vector<vec3> Derivate;
	vector<vec3> Tangent;
	vector<vec4> colorVertex;
	vector<int> index;
	vector<vec2> boudLimit;
	// Numero vertici
	int nv;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
} Figura;
