/*ELABORATO N.1 DIEGO GIUSEPPETTI*/
#include "ShaderMaker.h"
#include "Figura.h"
#include "creaVAO.h"
#include "Hermite.h"
#pragma region constant
#define  PI   3.14159265358979323846
const float w = 1280.0;
const float h = 720.0;

#pragma endregion

#pragma region Variabili_globali


//Inizializzazione parametri Tes, Bias, Cont per la modifica delle derivate agli estremi
float Tens = 0.0, Bias = 0.0, Cont = 0.0;  //Questa inizializzazione 'rappresenta le derivate come semplice rapporto incrementale
static unsigned int programId;
mat4 Projection, eye;
GLuint MatProj, MatModel;
unsigned int loctime, locres, locmouse; 
vec4 ViewPort,pos;
vec3 mousePos, posPrinc;

Figura Aereo, Sfondo, Nemico1, Nemico2, Nemico3;
vector<Figura*> Scena;
uint32_t mouse_status = 0;
float angleO;
float modelview[16];


#pragma endregion


#pragma region DefinizioneForme
void costruisciSfondo(vec4 color) {
	Sfondo.vertex.push_back(vec3(-1.0, -1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(1.0, -1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(1.0, 1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(1.0, 1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(-1.0, 1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
	Sfondo.vertex.push_back(vec3(-1.0, -1.0, -1.0));
	Sfondo.colorVertex.push_back(color);
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
	costruisci_formaHermite(color, fig);

}

void costruisciProiettile(Figura* fig, vec4 color) {
	int i;
	float stepA = (2 * PI) / fig->nv;
	float t;


	fig->vertex.push_back(vec3(0.0, 0.0, 0.0));
	fig->colorVertex.push_back(color);

	for (i = 0; i <= fig->nv / 2; i++)
	{
		t = (float)i * stepA;
		fig->vertex.push_back(vec3(cos(t), sin(t), 0.0));
		fig->colorVertex.push_back(color);
	}
}

#pragma endregion



float aggiornaAngolo(vec3 fig1, mat4 Mfig1, vec3 fig2, mat4 Mfig2) {
	//trovo coordinate globali
	vec4 pos1 = Mfig1 * vec4(fig1, 1.0);
	vec4 pos2 = Mfig2 * vec4(fig2, 1.0);

	//creo un nuovo sistema di riferimento in cui fig1 e il centro
	pos2.x = pos2.x - pos1.x;
	pos2.y = pos2.y - pos1.y;

	//calcolo angolo tra i due punti dati ponendo x = 0 gradi

	return atan( pos2.y/ pos2.x);

}

#pragma region INIT

void INIT_VAO(void)
{			
	Projection = ortho(0.0f, w, 0.0f, h);
	ViewPort = vec4(0.0f, 0.0f, w, h);
	posPrinc = vec3(w / 2, h/ 2, 0.0);
	//eye = mat4(1);
	//mousePos = vec3(0.0, 0.0, 0.0);
	//prendo da shader

	costruisciAereo(&Aereo, vec4(1.0, 1.0, 0.0, 1.0));	
	crea_VAO_Static(&Aereo);	
	Scena.push_back(&Aereo);

	costruisciAereo(&Nemico1, vec4(1.0, 0.0, 0.0, 1.0));
	
	crea_VAO_Static(&Nemico1);
	Scena.push_back(&Nemico1);

	costruisciAereo(&Nemico2, vec4(1.0, 0.0, 0.0, 1.0));
	
	crea_VAO_Static(&Nemico2);
	Scena.push_back(&Nemico2);

	costruisciAereo(&Nemico3, vec4(1.0, 0.0, 0.0, 1.0));
	
	crea_VAO_Hermite(&Nemico3);
	Scena.push_back(&Nemico3);

	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	loctime = glGetUniformLocation(programId, "time");
	locres = glGetUniformLocation(programId, "res");
	locmouse = glGetUniformLocation(programId, "mouse");




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
	
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	
	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));	
	glUniform2f(locres, w, h );


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	

	for (int k = 0; k < Scena.size(); k++)
	{

		if(k==0){//aereo bello
			Scena[k]->Model = mat4(1);
			Scena[k]->Model = translate(Scena[k]->Model, posPrinc);
			Scena[k]->Model = rotate(Scena[k]->Model, Scena[k]->AngoloRotazione, vec3(0.0, 0.0, 1.0));
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w / 7, h / 7, 1.0));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glBindVertexArray(Scena[k]->VAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nv);
		}
		else {

			Scena[k]->AngoloRotazione = aggiornaAngolo(Scena[0]->vertex[0], Scena[0]->Model,Scena[k]->vertex[0], Scena[k]->Model) ;
			Scena[k]->Model = mat4(1);
			Scena[k]->Model = translate(Scena[k]->Model, vec3(k*100, k*100, 0.0));
			Scena[k]->Model = rotate(Scena[k]->Model, Scena[k]->AngoloRotazione, vec3(0.0, 0.0, 1.0));
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w / 10, h / 10, 1.0));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glBindVertexArray(Scena[k]->VAO);
			if (Scena[k]->hasHerm) {
				glDrawArrays(GL_LINE_STRIP, 0, Scena[k]->nvHer);
			}
			glDrawArrays(GL_LINE_STRIP, 0, Scena[k]->nv);
		}

		
		
		glBindVertexArray(0);
	}
	//costruisci_formaHermite(color, &Aereo, &AereoPol);	
	
	
	//crea_VAO_Static(&Sfondo);
	//glBindVertexArray(Sfondo.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, Aereo.nv);
	//glBindVertexArray(0);
	


	glutSwapBuffers();

}

//prendo angolo tra nemico e personaggio per far si che fi giri verso lui


void spara() {
	//costruisciProiettile()

}

void seguiMouse(int x, int y) {

	
	mousePos = vec3(x, y, 0.0);
	Aereo.AngoloRotazione = aggiornaAngolo(mousePos,mat4(1),Aereo.vertex[0],Aereo.Model);
	
	glutPostRedisplay();
}

void onMouseButton(int button, int state, int x, int y)
{
	
	switch (button) {
	case GLUT_LEFT_BUTTON:      
		spara();
		break;
	case GLUT_MIDDLE_BUTTON:  
		break;
	case GLUT_RIGHT_BUTTON:    
		break;
	}

	

	glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y)
{
	{
		switch (key)
		{

		case ' ':
			
			break;

		case 'w':
			posPrinc.y +=2;
			break;
		case 'a':
			posPrinc.x-=2;
			break;

		case 's':
			posPrinc.y-=2;
			break;
		case 'd':
			posPrinc.x+=2;
			break;

		
		default:
			break;
		}
	}
	glutPostRedisplay();
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
	glutMouseFunc(onMouseButton);
	glutPassiveMotionFunc(seguiMouse);//chiama funzione per tutto il tempo in cui il mouse e sopra la finestra
	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, w, h);
	glutMainLoop();
}