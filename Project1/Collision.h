#pragma once
#include "Figura.h"
bool checkCollision(Figura* fig1, Figura* fig2); // controlla le collisioni tra le due figure passate
void calculateBoundingBox(Figura* fig); // calcola i limiti delle bounding box in coordinate locali dell'oggetto
void updateGlobalCollisionCoordinates(Figura* fig); // trasforma le coordinate locali in coordinate globali premoltiplicando per la matrice di trasformazione
