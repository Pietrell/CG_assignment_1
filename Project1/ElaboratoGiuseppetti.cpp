
#include "ShaderMaker.h"
#include "Figura.h"
#include "creaVAO.h"
#include "Hermite.h"

#pragma region constant
#define  PI   3.14159265358979323846


const float w = 1280.0;
const float h = 720.0;
#pragma endregion

#pragma region Variabili


//Inizializzazione parametri Tes, Bias, Cont per la modifica delle derivate agli estremi
float Tens = 0.0, Bias = 0.0, Cont = 0.0;  //Questa inizializzazione 'rappresenta le derivate come semplice rapporto incrementale
static unsigned int programId;
mat4 Projection;
GLuint MatProj, MatModel;
unsigned int loc_time, loc_res, loc_mouse; 
vec2 res;
Figura Aereo, AereoPol;

#pragma endregion




#pragma region INIT

void INIT_VAO(void)
{
	//definisco matrici di trasformazione comuni a tutti gli elementi
		
	Projection = ortho(0.0f, w, 0.0f, h);
	//prendo da shader
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
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


#pragma region DefinizioneForme

void costruisciAereo(Figura* fig, vec4 color) {

	fig->vertex.push_back(vec3(1.0, 1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.2, 0.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-1.0, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(-0.2, -1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.2, -1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(0.2, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(1.0, -0.8, 0.0));
	fig->colorVertex.push_back(color);
	fig->vertex.push_back(vec3(1.0, 1.0, 0.0));
	fig->colorVertex.push_back(color);
	fig->nv = fig->vertex.size();
	
}


#pragma endregion

void drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	vec4 color = vec4(1.0,1.0,1.0,1.0);
	costruisciAereo(&Aereo,color);

	//costruisci_formaHermite(color, &Aereo, &AereoPol);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glUniform2f(loc_res, res.x, res.y);

	
	crea_VAO_Static(&Aereo);
	glBindVertexArray(Aereo.VAO);
	glDrawArrays(GL_LINE_STRIP, 0, Aereo.vertex.size());
	glBindVertexArray(0);



	glutSwapBuffers();

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(w, h);
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
	glViewport(0, 0, w, h);
	glutMainLoop();
}