#pragma once
#include "Figura.h"

#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)




float dx(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig);
float dy(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig);

void InterpolazioneHermite(float* t, Figura* Fig, vec4 color);
void costruisci_formaHermite(vec4 color, Figura* forma);
