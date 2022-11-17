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
mat4 Projection;
GLuint MatProj, MatModel;
unsigned int loctime, locres, locmouse; 
vec4 pos;
vec2 res;
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

float aggiornaAngolo(vec2 fig1, mat4 Mfig1, vec2 fig2, mat4 Mfig2) {



	//prodotto scalare tra posizioni (posAereo.x - posNemico.x) + (posAereo.y - posNemico.y)
	vec4 pos1 =   Mfig1* vec4(fig1.x, fig1.y, 0.0, 1.0);
	vec4 pos2 =  Mfig2 * vec4(fig2.x, fig2.y, 0.0, 1.0);

	return ( cos(glm::dot(pos1, pos2)) - radians(90.0) );
}

#pragma region INIT

void INIT_VAO(void)
{			
	Projection = ortho(0.0f, w, 0.0f, h);
	
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
			Scena[k]->Model = translate(Scena[k]->Model, vec3(w / 2, h / 2, 0.0));
			Scena[k]->Model = rotate(Scena[k]->Model, Scena[k]->AngoloRotazione, vec3(0.0, 0.0, 1.0));
			Scena[k]->Model = scale(Scena[k]->Model, vec3(w / 7, h / 7, 1.0));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k]->Model));
			glBindVertexArray(Scena[k]->VAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[k]->nv);
		}
		else {

			Scena[k]->Model = mat4(1);
			Scena[k]->AngoloRotazione = aggiornaAngolo(Scena[0]->vertex[0], Scena[0]->Model,Scena[k]->vertex[0], Scena[k]->Model);
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
	cout << "pre calcolo";
	Aereo.AngoloRotazione = aggiornaAngolo(vec2(x, y), mat4(1), Aereo.vertex[0], Aereo.Model);
	//pos = Projection * vec4(x, y, 0.0, 1.0); //trasformo posizione del mouse in coordinate locali
	//pos.y *= -1; //per qualche motivo y invertita
	////printf("%f, %f \n", pos.x, pos.y);
	//Aereo.AngoloRotazione = atan2(pos.y, pos.x);
	//Aereo.AngoloRotazione -= radians(90.0);//offset di rotazione => voglio x=0 come 0 invece di y
	cout << Aereo.AngoloRotazione;
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