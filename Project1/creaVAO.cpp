#include "creaVAO.h"


void crea_VAO_Static(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_Vert);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, fig->vertex.size() * sizeof(vec3), fig->vertex.data(), GL_STATIC_DRAW);
	//carico VBO vertici nel layer 1s
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	//VBO dei colori
	glGenBuffers(1, &fig->VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, fig->colorVertex.size() * sizeof(vec4), fig->colorVertex.data(), GL_STATIC_DRAW);
	// carico VBO colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void crea_VAO_Hermite(Figura* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_Vert);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, fig->Hermite.size() * sizeof(vec3), fig->Hermite.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, fig->colorHermite.size() * sizeof(vec4), fig->colorHermite.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


}

void crea_VAO_Dynamic(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
	glGenBuffers(1, &fig->VBO_Vert);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, fig->vertex.size() * sizeof(vec3), fig->vertex.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
	glGenBuffers(1, &fig->VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, fig->colorVertex.size() * sizeof(vec4), fig->colorVertex.data(), GL_DYNAMIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}