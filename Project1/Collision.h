#pragma once
#include <iostream>
#include "Lib.h"
#include "Figura.h"

typedef struct {
	vec4 corner_b_obj;
	vec4 corner_t_obj;
	vec4 corner_b;
	vec4 corner_t;
}BoundingBox;

void calculateBoundingBox(Figura* fig);
bool checkCollision(BoundingBox box);
void updateModel(mat4 matModel);
void addVertices(vector<vec3> vertici);