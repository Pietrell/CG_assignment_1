#pragma once

#include "Figura.h"

bool checkCollision(BoundingBox obj1, BoundingBox obj2);
void addVertices(vector<vec3> vertici);
void calculateBoundingBox(Figura* fig);



void updateModel(mat4 matModel);
