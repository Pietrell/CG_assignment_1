#include <iostream>
#include "ShaderMaker.h"
#include "Lib.h"


#pragma region Variables


// viewport size
int width = 1280;
int height = 720;
int selected_obj = -1;

//Inizializzazione Menu
int submenu_Opzioni_I, menu_id, submenu_SplineC1Interp, submenu_Bezier;

//Inizializzazioni
int mod_par_der = 0;  //(0) 1 : (non) si intende agire sui parametri T,B,C nel calcolo numerico della derivata nel vertice di controllo selezionato; 
int visualizzaTg = 0; //(0) 1 : (non) si intende visualizzare graficamente la tangente nei vertici di controllo
int visualizzaPC = 0; //(0) 1 : (non) si intende visualizzare graficamente il poligono di controllo

//Inizializzazione parametri Tes, Bias, Cont per la modifica delle derivate agli estremi
float Tens = 0.0, Bias = 0.0, Cont = 0.0;  //Questa inizializzazione 'rappresenta le derivate come semplice rapporto incrementale

int metodo = 1;  //Metodo=1 --> interpolazione curve di Hermite;  Metodo=2 --> approssimazione di forma curva di Bezier

int M_I = 0;
static unsigned int programId;
mat4 Projection;
GLuint MatProj, MatModel;
unsigned int lsceltavs, lsceltafs, loc_time, loc_res, loc_mouse;
vec2 res, mouse;

int pval = 140;
float* t;
#define  PI   3.14159265358979323846
typedef struct {
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint EBO_indici;
	
	// Vertici	
	vector<vec3> CP;	
	vector<vec3> Derivata;
	vector<vec4> colCP;
	vector<int> indici;
	// Numero vertici
	int nv;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	int sceltaVS;
	int sceltaFS;
	string name;
} Figura;


Figura Curva, Poligonale, Derivata, tangenti;
Figura Aereo, Risultato;
#pragma endregion


#pragma region crea VAO

void crea_VAO_Vector(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->CP.size() * sizeof(vec3), fig->CP.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colCP.size() * sizeof(vec4), fig->colCP.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}

void crea_VAO_CP(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->CP.size() * sizeof(vec3), fig->CP.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colCP.size() * sizeof(vec4), fig->colCP.data(), GL_DYNAMIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}
#pragma endregion

#pragma region hermite
/// /////////////////////////////////// Disegna geometria //////////////////////////////////////
//Per Curve di hermite
#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)

float dx(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig)
{
	if (i == 0)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i + 1].x - Fig->CP[i].x) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i].x - Fig->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
}

float dy(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float* t, Figura* fig)
{
	//Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente x della curva � quello originale, altrimenti � quello che � stato modificato nella funzione 
	//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (fig->Derivata.at(i).x == 0)
		return dx(i, t, 0.0, 0.0, 0.0, &Poligonale);

	if (fig->Derivata.at(i).x != 0)
		return fig->Derivata.at(i).x;

}

float DY(int i, float* t, Figura* fig)
{
	// Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente y della curva � quello originale, altrimenti � quello che � stato modificato nella funzione
		//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (fig->Derivata.at(i).y == 0)
		return dy(i, t, 0.0, 0.0, 0.0, &Poligonale);

	if (fig->Derivata.at(i).y != 0)
		return fig->Derivata.at(i).y;

}

void InterpolazioneHermite(float* t, Figura* Fig, vec4 color_top, vec4 color_bot)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);

	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; //indice dell'estremo sinistro dell'intervallo [t(i),t(i+1)] a cui il punto tg
	//appartiene


	Fig->CP.clear();
	Fig->colCP.clear();
	tangenti.CP.clear();

	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1]) is++;

		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		x = Fig->CP[is].x * PHI0(tgmapp) + DX(is, t, Fig) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t,Fig) * PSI1(tgmapp) * ampiezza;
		y = Fig->CP[is].y * PHI0(tgmapp) + DY(is, t, Fig) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t, Fig) * PSI1(tgmapp) * ampiezza;
		Fig->CP.push_back(vec3(x, y, 0.0));
		Fig->colCP.push_back(color_top);
	}

}

#pragma endregion

#pragma region Controllo mouse
/*
void mouseMotion(int x, int y)
{

	float mousex = x;
	float mousey = height - y;


	if (M_I == 1)
	{
		if (selected_obj >= 0)
		{
			printf("Ho selezionato il Punto selezionato %d \n", selected_obj);
			Curva.CP[selected_obj] = vec3(mousex, mousey, 0.0);
			Curva.colCP[selected_obj] = vec4(0.0, 0.0, 1.0, 1.0);
		}
	}
	glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y)
{
	int i;
	float dist, dist1;
	float TOLL = 10;  //Tolleranza per la selezione del vertice di controllo da modificare

	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		//case GLUT_LEFT_BUTTON:
		//	float mousex = (float)x;
		//	float mousey = (float)height - y;
		//	if (M_I == 1 || mod_par_der == 1)
		//	{
		//		selected_obj = 0;
		//		dist = sqrt((mousex - Curva.CP[0].x) * (mousex - Curva.CP[0].x) + (mousey - Curva.CP[0].y) * (mousey - Curva.CP[0].y));
		//		//Calcolo la distanza tra il punto in cui si trova attualmente il mouse e tutti i vertici di controllo sulla finestre
		//	   //ed individuo l'indice selected_obj del vertice di controllo pi� vicino alla posizione del mouse
		//		for (i = 1; i < Curva.CP.size(); i++) {
		//			dist1 = sqrt((mousex - Curva.CP[i].x) * (mousex - Curva.CP[i].x) + (mousey - Curva.CP[i].y) * (mousey - Curva.CP[i].y));
		//			if (dist1 < dist) {
		//				selected_obj = i;
		//				dist = dist1;
		//			}
		//		}
		//		if (dist > TOLL)
		//			selected_obj = -1;
		//		Tens = 0.0;
		//		Cont = 0.0;
		//		Bias = 0.0;
		//		printf("Punto selezionato %d \n", selected_obj);
		//	}
		//	else if (M_I == 0)
		//	{
		//		Curva.CP.push_back(vec3(mousex, mousey, 0.0));
		//		Curva.colCP.push_back(vec4(1.0, 0.0, 0.0, 1.0));
		//		//Ad ogni vertice di controllo associo un valore vec3(0.0,0.0,0.0) per indicare che la derivata in quel vertice di controllo non � stata modificata
		//		Derivata.CP.push_back(vec3(0.0, 0.0, 0.0));
		//	}

		default:
			break;
		}

	}


	glutPostRedisplay();
}

*/
#pragma endregion

#pragma region Controllo tastiera

void mykeyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	default:
		break;
	}

	//Modifico la derivata del vertice selezionato con valori Tens, cont e Bias selezionati da tastiera
	Derivata.CP.at(selected_obj).x = dx(selected_obj, t, Tens, Bias, Cont, &Poligonale);
	Derivata.CP.at(selected_obj).y = dy(selected_obj, t, Tens, Bias, Cont, &Poligonale);

	glutPostRedisplay();

}
#pragma endregion

#pragma region costruisci Forma

void costruisci_aereo(vec4 color, Figura* forma) {
	
	Aereo.CP.push_back(vec3(1.0, 1.0, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(-0.2, 0.0, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(-1.0, -0.8, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(-0.2, -1.0, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(0.2, -1.0, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(0.2, -0.8, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(1.0, -0.8, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.CP.push_back(vec3(1.0, 1.0, 0.0));
	Aereo.colCP.push_back(vec4(color.r, color.g, color.b, color.a));
	Aereo.nv = Aereo.CP.size();


	for (int i = 0; i < Aereo.CP.size(); i++)
	{
		Aereo.Derivata.push_back(vec3(0.0, 0.0, 0.0));
	}

	return;
}

void costruisci_formaHermite(vec4 color_top, vec4 color_bot, Figura* forma)
{

	Poligonale.CP = forma->CP;
	Poligonale.colCP = forma->colCP;

	if (Poligonale.CP.size() > 1)
	{
		t = new float[forma->CP.size()];
		int i;
		float step = 1.0 / (float)(forma->CP.size() - 1);

		for (i = 0; i < forma->CP.size(); i++)
			t[i] = (float)i * step;


		Risultato.Derivata = forma->Derivata;
		InterpolazioneHermite(t, &Risultato, color_top, color_bot);

		forma->nv = Curva.CP.size();
	}


}
#pragma endregion

#pragma region INIT

void INIT_VAO(void)
{

	Curva.sceltaFS = 0;
	Curva.sceltaVS = 0;
	Curva.name = "Curva";

	Poligonale.sceltaFS = 0;
	Poligonale.sceltaVS = 0;
	Poligonale.name = "Poligonale";

	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	lsceltavs = glGetUniformLocation(programId, "sceltaVS");
	lsceltafs = glGetUniformLocation(programId, "sceltaFS");

	loc_time = glGetUniformLocation(programId, "time");
	loc_res = glGetUniformLocation(programId, "res");
	loc_mouse = glGetUniformLocation(programId, "mouse");
}

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

#pragma endregion


void drawScene(void)
{



	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	vec4 col_bottom = vec4{ 0.5451, 0.2706, 0.0745, 1.0000 };
	vec4 col_top = vec4{ 1.0,0.4980, 0.0353,1.0000 };

	
	costruisci_aereo(col_bottom, &Aereo);
	Curva.CP = Aereo.CP;
	Curva.colCP = Aereo.colCP;
	Curva.nv = Aereo.nv;
	Curva.Derivata = Aereo.Derivata;
	
		costruisci_formaHermite(col_top, col_bottom, &Curva);
		crea_VAO_Vector(&Curva);
		crea_VAO_CP(&Poligonale);
	




	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glUniform2f(loc_res, res.x, res.y);
	glUniform2f(loc_mouse, mouse.x, mouse.y);

	glUniform1i(lsceltavs, Curva.sceltaVS);
	glUniform1i(lsceltafs, Curva.sceltaFS);
	//la matrice di Modellazione della Curva coincide con la matrice identit�, perch� selezioniamo i punti con il mouse in coordinate del mondo
	Curva.Model = mat4(1.0);

	glPointSize(6.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Curva.Model));

	//Disegno la curVa se ho pi� di 2 vertici di controllo
	
		glBindVertexArray(Curva.VAO);
		glDrawArrays(GL_LINE_STRIP, 0, Curva.CP.size());
		glBindVertexArray(0);
	

	//Disegno i vertici di controllo
	glBindVertexArray(Poligonale.VAO);
	glDrawArrays(GL_POINTS, 0, Poligonale.CP.size());
	glBindVertexArray(0);


	glutSwapBuffers();

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Editor Curve");
	glutDisplayFunc(drawScene);
	//gestione animazione
	//glutMouseFunc(onMouse);
	//glutKeyboardFunc(mykeyboard);


	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glutMotionFunc(mouseMotion);

	glutMainLoop();
}