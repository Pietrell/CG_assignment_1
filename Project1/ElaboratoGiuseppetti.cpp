/*ELABORATO N.1 DIEGO GIUSEPPETTI*/
#include "ShaderMaker.h"
#include "Figura.h"
#include "creaVAO.h"
#include "Hermite.h"
#include "Collision.h"
#include <math.h>

#pragma region constant
#define  PI   3.14159265358979323846
#define SCALE (float)30
const float w = 1280.0;
const float h = 720.0;
#pragma endregion

#pragma region Variabili_globali
//Inizializzazione parametri Tes, Bias, Cont per la modifica delle derivate agli estremi
float Tens = 0.0, Bias = 0.0, Cont = 0.0;  //Questa inizializzazione 'rappresenta le derivate come semplice rapporto incrementale
static unsigned int programId;
mat4 Projection, eye;
GLuint MatProj, MatModel;
unsigned int loctime, locres, locmouse, lsceltafs;
int principalIndex;
vec4 pos;
Figura Aereo, Sfondo, Nemico1, Nemico2, Nemico3;
vector<Figura*> Scena;
uint32_t mouse_status = 0;
float angleO;
float modelview[16];
#pragma endregion


#pragma region DefinizioneForme
void costruisciSfondo(vec4 color) {
	Sfondo.vertex.push_back(vec3(0.0, 0.0, 0.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(0.0, 1.0, 0.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(1.0, 0.0, 0.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(1.0, 1.0, 0.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.nv = 4;
}


void costruisciAereo(Figura* fig, vec4 color) {

	fig->vertex.push_back(vec3(0.0, 0.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.0, 1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.2, 0.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.6, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.2, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.2, -1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.2, -1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.2, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.6, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.2, 0.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.0, 1.0, 0.0));
	fig->colorVertex.push_back(color);

	fig->nv = fig->vertex.size();
	fig->AngoloRotazione = 0.0f;
	fig->hasHerm = false;
	Aereo.draw = true;
	costruisci_formaHermite(color, fig);

}

#pragma endregion

float aggiornaAngolo(vec3 fig1, mat4 Mfig1, vec3 fig2, mat4 Mfig2) {
	//trovo coordinate globali
	vec4 pos1 = Mfig1 * vec4(fig1, 1.0);
	vec4 pos2 = Mfig2 * vec4(fig2, 1.0);

	//creo un nuovo sistema di riferimento in cui fig1 e il centro
	pos2.x = (pos2.x - pos1.x);
	pos2.y = (pos2.y - pos1.y);
	
	//calcolo angolo tra i due punti dati ponendo x = 0 gradi
	double radian = atan(pos2.y / pos2.x);
	if (pos2.x >= 0) {
		 radian += radians(180.0);
	}
	return radian - radians(90.0); // risultato ottenuto mappa l'asse delle ascisse
}

void nextEnemyMov(Figura* enemy, Figura* prin) {
	
	//ottengo coordinate finestra
	vec4 posPr = prin->Model * vec4(prin->vertex[0], 1.0);
	vec4 posEn = enemy->Model * vec4(enemy->vertex[0], 1.0);	
	vec2 delta = vec2(posPr.x - posEn.x, posPr.y - posEn.y);
	//ottengo segno per cui moltiplicare lo step (come nel piano cartesiano)
	posPr.x -= posEn.x;
	posPr.y -= posEn.y;	
	//a seconda del risultato ottenuto sopra sottraggo o sommo lo step alle coordinate
	posEn.x += posPr.x > 0 ? +2 : -2;
	posEn.y += posPr.y > 0 ? +2 : -2;
	//aggiorno coordinate globali
	enemy->globalPos.x = posEn.x;
	enemy->globalPos.y = posEn.y;
}

#pragma region INIT

void INIT_VAO(void)
{			
	srand(time(NULL)); //inposto funzione random in base al tempo
	Projection = ortho(0.0f, w, 0.0f, h);

	//0 : sfondo
	costruisciSfondo(vec4(0.3,0.3,0.3,0.1));
	Sfondo.sceltaFS = 1;
	Sfondo.Model = mat4(1);
	Sfondo.Model = scale(Sfondo.Model, vec3(w , h , 1.0));
	Sfondo.draw = true;
	crea_VAO_Static(&Sfondo);
	Scena.push_back(&Sfondo);

	//1 : personaggio principale
	costruisciAereo(&Aereo, vec4(1.0, 1.0, 1.0, 0.1));	
	Aereo.globalPos = vec3(w / 2, h / 2, 0.0);
	Aereo.Model = mat4(1);
	Aereo.Model = translate(Aereo.Model, Aereo.globalPos);
	Aereo.Model = rotate(Aereo.Model, Aereo.AngoloRotazione, vec3(0.0, 0.0, 1.0));
	Aereo.Model = scale(Aereo.Model, vec3(w/SCALE, h/SCALE, 1.0));
	Aereo.sceltaFS = 2;
	principalIndex = 1;
	Aereo.draw = true;
	calculateBoundingBox(&Aereo);
	crea_VAO_Static(&Aereo);
	Scena.push_back(&Aereo);

	//primo aereo nemico
	costruisciAereo(&Nemico1, vec4(1.0, 0.0, 0.0, 1.0));
	Nemico1.globalPos = vec3(rand()%(int)w, rand()%(int)h, 0.0);		
	Nemico1.Model = mat4(1);
	Nemico1.Model = translate(Nemico1.Model, Nemico1.globalPos);
	Nemico1.Model = rotate(Nemico1.Model, 0.0f, vec3(0.0, 0.0, 1.0));
	Nemico1.Model = scale(Nemico1.Model, vec3(w / SCALE, h / SCALE, 1.0));
	Nemico1.sceltaFS = 0;
	Nemico1.draw = true;
	calculateBoundingBox(&Nemico1);
	crea_VAO_Hermite(&Nemico1);	
	Scena.push_back(&Nemico1);


	//secondo aereo nemico
	costruisciAereo(&Nemico2, vec4(1.0, 0.0, 0.0, 1.0));
	Nemico2.globalPos = vec3(rand() % (int)w, rand() % (int)h, 0.0);	
	Nemico2.Model = mat4(1);
	Nemico2.Model = translate(Nemico2.Model, Nemico2.globalPos);
	Nemico2.Model = rotate(Nemico2.Model, 0.0f, vec3(0.0, 0.0, 1.0));
	Nemico2.Model = scale(Nemico2.Model, vec3(w / SCALE, h / SCALE, 1.0));
	Nemico2.sceltaFS = 0;
	Nemico2.draw = true;
	calculateBoundingBox(&Nemico2);
	crea_VAO_Hermite(&Nemico2);
	Scena.push_back(&Nemico2);


	//terzo aereo nemico
	costruisciAereo(&Nemico3, vec4(1.0, 0.0, 0.0, 1.0));
	Nemico3.globalPos = vec3(rand() % (int)w, rand() % (int)h, 0.0);
	Nemico3.Model = mat4(1);
	Nemico3.Model = translate(Nemico3.Model, Nemico3.globalPos);
	Nemico3.Model = rotate(Nemico3.Model, 0.0f, vec3(0.0, 0.0, 1.0));
	Nemico3.Model = scale(Nemico3.Model, vec3(w / SCALE, h / SCALE, 1.0));
	Nemico3.sceltaFS = 0;
	Nemico3.draw = true;
	calculateBoundingBox(&Nemico3);
	crea_VAO_Hermite(&Nemico3);
	Scena.push_back(&Nemico3);

	//passaggio dati a shader
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	loctime = glGetUniformLocation(programId, "time");
	locres = glGetUniformLocation(programId, "resolution");
	locmouse = glGetUniformLocation(programId, "mouse");
	lsceltafs = glGetUniformLocation(programId, "sceltaFS");
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
	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));	
	glUniform2f(locres, w, h );
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);	

	for (int k = 0; k < Scena.size(); k++)
	{
		if (Scena[k]->draw == false)
			break; // se non devo disegnare l'elemento passo al prossimo
		if (k==0) // sfondo
		{
			Scena[k]->Model = mat4(1);
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w, h, 1.0));
			glUniform1i(lsceltafs, Scena[k]->sceltaFS);
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glBindVertexArray(Scena[k]->VAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, Scena[k]->nv);
		}

		if(k==1){	//personaggio principale
			

			Scena[k]->Model = mat4(1);
			Scena[k]->Model = translate(Scena[k]->Model, Scena[k]->globalPos);
			Scena[k]->Model = rotate(Scena[k]->Model, Scena[k]->AngoloRotazione, vec3(0.0, 0.0, 1.0));
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w/SCALE, h/SCALE, 1.0));
			updateGlobalCollisionCoordinates(Scena[k]);
			
			glUniform1i(lsceltafs, Scena[k]->sceltaFS);
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glBindVertexArray(Scena[k]->VAO);
			if (Scena[k]->hasHerm) {
				glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nvHer);
			}else{

				glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nv);
			}
		
		}
		if(k>1) { //nemici
			
			
			
			if (Scena[principalIndex]->draw) { //sposto solo se il personaggio principale e ancora vivo
				nextEnemyMov(Scena[k], Scena[principalIndex]);
			}
			Scena[k]->AngoloRotazione = aggiornaAngolo(Scena[principalIndex]->vertex[0], Scena[principalIndex]->Model, Scena[k]->vertex[0], Scena[k]->Model);
			Scena[k]->Model = mat4(1);
			Scena[k]->Model = translate(Scena[k]->Model, Scena[k]->globalPos);
			Scena[k]->Model = rotate(Scena[k]->Model, Scena[k]->AngoloRotazione, vec3(0.0, 0.0, 1.0));
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w / SCALE, h / SCALE, 1.0));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glUniform1i(lsceltafs, Scena[k]->sceltaFS);
			glBindVertexArray(Scena[k]->VAO);
			
			if (Scena[k]->hasHerm) {
				glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nvHer);
			}
			else {

			glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nv);
			}
		}		
		glBindVertexArray(0);

		for (int k = 2; k < Scena.size(); k++)
		{
			if (checkCollision(Scena[1], Scena[k])) {
				Scena[principalIndex]->draw = false;
				cout << "colpito" << endl;
			}
		}
	}
	glutSwapBuffers();
}

//prendo angolo tra nemico e personaggio per far si che fi giri verso lui

#pragma region controlliUtente



void seguiMouse(int x, int y) {
	Aereo.AngoloRotazione = aggiornaAngolo(vec3(x,h-y,0.0), mat4(1), Aereo.vertex[0], Aereo.Model);
}

void myKeyboard(unsigned char key, int x, int y)
{
	{
		switch (key)
		{
		case 'w':
			Aereo.globalPos.y +=5;
			break;
		case 'a':
			Aereo.globalPos.x -=5;
			break;

		case 's':
			Aereo.globalPos.y -=5;
			break;
		case 'd':
			Aereo.globalPos.x +=5;
			break;
		default:
			break;
		}
	}

}
#pragma endregion
void frame(int a) {
	glutPostRedisplay();
	glutTimerFunc(60, frame, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Elaborato Giuseppetti");
	glutDisplayFunc(drawScene);

	//gestione animazione
	glutKeyboardFunc(myKeyboard);
	glutPassiveMotionFunc(seguiMouse);//chiama funzione per tutto il tempo in cui il mouse e sopra la finestra
	glutTimerFunc(40,frame,0); // aggionra con frequenza di 25 FPS

	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glViewport(0, 0, w, h);
	glutMainLoop();
}