
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
unsigned int loctime, locres, locmouse; 
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


#pragma region DefinizioneForme

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


	fig->Model = mat4(1.0);
	fig->Model = translate(fig->Model, vec3(w / 3, h / 3, 0.0));
	fig->Model = rotate(fig->Model, radians(60.0f), vec3(0.0,0.0,1.0));
	fig->Model = scale(fig->Model, vec3(w/5, h/5, 1.0));
}


#pragma endregion

void drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	vec2 resolution = vec2((float)w, (float)h);
	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));	
	glUniform2f(locres, resolution.x, resolution.y);

	vec4 color = vec4(1.0,1.0,0.0,1.0);
	costruisciAereo(&Aereo,color);
	//costruisci_formaHermite(color, &Aereo, &AereoPol);	
	crea_VAO_Static(&Aereo);


	
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Aereo.Model));
	glBindVertexArray(Aereo.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, Aereo.nv);
	glBindVertexArray(0);
	


	glutSwapBuffers();

}

void modifyModelMatrix(Figura* fig, glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	//ricordare che mat4(1) costruisce una matrice identità di ordine 4
	mat4 traslation = glm::translate(glm::mat4(1), translation_vector);
	mat4 rotation = glm::rotate(glm::mat4(1), angle, rotation_vector);
	mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scale_factor, scale_factor, scale_factor));

	//Modifica la matrice di Modellazione dell'oggetto della scena selezionato postmolitplicando per le matrici scale*rotarion*traslation

	fig->Model = fig->Model * scale * rotation * traslation;

	

}

void seguiMouse(int x, int y) {
	vec2 pos = Aereo.vertex[0];
	float angle_ = atan2(pos.y - y, pos.x - x);
	
	modifyModelMatrix(&Aereo, vec3(0.0), vec3(0.0,0.0,1.0), angle_, 0);

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
	//glutMouseFunc(onMouse);
	//glutKeyboardFunc(mykeyboard);
	glutPassiveMotionFunc(seguiMouse);
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