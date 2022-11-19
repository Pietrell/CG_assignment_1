#pragma once

#include "Figura.h"

bool checkCollision(Figura* fig1, Figura* fig2);
void calculateBoundingBox(Figura* fig);
void updateGlobalCoordinates(Figura* fig);
