#include "Hermite.h"

int pval = 140;
float* t;

float dx(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig)
{
	if (i == 0)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->vertex[i + 1].x - Fig->vertex[i].x) / (t[i + 1] - t[i]);
	if (i == Fig->vertex.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->vertex[i].x - Fig->vertex[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->vertex.at(i).x - Fig->vertex.at(i - 1).x) / 
			(t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->vertex.at(i + 1).x - Fig->vertex.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->vertex.at(i).x - Fig->vertex.at(i - 1).x) / 
			(t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->vertex.at(i + 1).x - Fig->vertex.at(i).x) / (t[i + 1] - t[i]);
}

float dy(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (Fig->vertex.at(i + 1).y - Fig->vertex.at(i).y) / (t[i + 1] - t[i]);
	if (i == Fig->vertex.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->vertex.at(i).y - Fig->vertex.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->vertex.at(i).y - Fig->vertex.at(i - 1).y) / 
			(t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->vertex.at(i + 1).y - Fig->vertex.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->vertex.at(i).y - Fig->vertex.at(i - 1).y) / 
			(t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->vertex.at(i + 1).y - Fig->vertex.at(i).y) / (t[i + 1] - t[i]);
}


void InterpolazioneHermite(float* t, Figura* Fig, vec4 color)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);
	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; //indice dell'estremo sinistro dell'intervallo [t(i),t(i+1)] a cui il punto tg appartiene

	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1]) {
			is++;
		}
		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;
		x = Fig->vertex[is].x * PHI0(tgmapp) + dx(is, t, p_t, p_b,p_c, Fig) * PHI1(tgmapp) * ampiezza + Fig->vertex[is + 1].x * PSI0(tgmapp) + dx(is + 1, t, p_t, p_b, p_c, Fig) * PSI1(tgmapp) * ampiezza;
		y = Fig->vertex[is].y * PHI0(tgmapp) + dy(is, t, p_t, p_b, p_c, Fig) * PHI1(tgmapp) * ampiezza + Fig->vertex[is + 1].y * PSI0(tgmapp) + dy(is + 1, t, p_t, p_b, p_c, Fig) * PSI1(tgmapp) * ampiezza;
		Fig->Hermite.push_back(vec3(x, y, 0.0));
		Fig->colorHermite.push_back(color);
	}	
}


void costruisci_formaHermite(vec4 color, Figura* forma )
{
	t = new float[forma->vertex.size()];
	int i;
	float step = 1.0 / (float)(forma->vertex.size() - 1);
	for (i = 0; i < forma->vertex.size(); i++) {
		t[i] = (float)i * step;
	}
	InterpolazioneHermite(t, forma, color);
	forma->hasHerm = true;
	forma->nvHer = forma->Hermite.size();	
}