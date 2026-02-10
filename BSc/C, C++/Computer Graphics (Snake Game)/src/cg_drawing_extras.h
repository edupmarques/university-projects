#pragma once

#include "ofMain.h"
#include "cg_extras.h"

// Função que desenha malha unitária com resolução mxn
inline void malha_unit(GLint m, GLint n, GLint size) {
	GLfloat x_start = -0.5;
	GLfloat y_start = -0.5;
	GLfloat x_step = 1.0 / GLfloat(m);
	GLfloat y_step = 1.0 / GLfloat(n);
	GLfloat imgX = size / GLfloat(m);
	GLfloat imgY = size / GLfloat(n);
	glEnable(GL_TEXTURE);
	glBegin(GL_QUADS);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			GLfloat minX = i * imgX;
			GLfloat maxX = (i + 1) * imgX;
			GLfloat minY = j * imgY;
			GLfloat maxY = (j + 1) * imgY;
			glTexCoord2f(minX, minY); glVertex2d(i * x_step + x_start, j * y_step + y_start);
			glTexCoord2f(minX, maxY); glVertex2d(i * x_step + x_start, (j + 1) * y_step + y_start);
			glTexCoord2f(maxX, maxY); glVertex2d((i + 1) * x_step + x_start, (j + 1) * y_step + y_start);
			glTexCoord2f(maxX, minY); glVertex2d((i + 1) * x_step + x_start, j * y_step + y_start);
		}
	}
	glEnd();
}

// Função que desenha um cubo
inline void cube_unit(GLint size) {
	GLfloat p = 0.5;
	glBegin(GL_QUADS);

	//frente
	glTexCoord2f(0, 0); glVertex3f(-p, -p, p);
	glTexCoord2f(size, 0); glVertex3f(-p, p, p);
	glTexCoord2f(size, size); glVertex3f(p, p, p);
	glTexCoord2f(0, size); glVertex3f(p, -p, p);

	//tras
	glTexCoord2f(0, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(size, 0); glVertex3f(p, -p, -p);
	glTexCoord2f(size, size); glVertex3f(p, p, -p);
	glTexCoord2f(0, size); glVertex3f(-p, p, -p);

	//cima
	glTexCoord2f(0, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(size, 0); glVertex3f(-p, -p, p);
	glTexCoord2f(size, size); glVertex3f(p, -p, p);
	glTexCoord2f(0, size); glVertex3f(p, -p, -p);

	//baixo
	glTexCoord2f(0, 0); glVertex3f(-p, p, p);
	glTexCoord2f(size, 0); glVertex3f(-p, p, -p);
	glTexCoord2f(size, size); glVertex3f(p, p, -p);
	glTexCoord2f(0, size); glVertex3f(p, p, p);

	//esq
	glTexCoord2f(0, 0); glVertex3f(-p, -p, p);
	glTexCoord2f(size, 0); glVertex3f(-p, -p, -p);
	glTexCoord2f(size, size); glVertex3f(-p, p, -p);
	glTexCoord2f(0, size); glVertex3f(-p, p, p);

	//dir
	glTexCoord2f(0, 0); glVertex3f(p, -p, p);
	glTexCoord2f(size, 0); glVertex3f(p, p, p);
	glTexCoord2f(size, size); glVertex3f(p, p, -p);
	glTexCoord2f(0, size); glVertex3f(p, -p, -p);

	glEnd();
}