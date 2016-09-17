#include "glsupport.h"
#include <glut.h>


GLint program;
GLuint vertPostionVBO;
GLuint positionAttribute;

GLuint vertColorVBO;
GLuint colorAttribute;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);

	glutSwapBuffers();
}

void init() {
	glClearColor(0.2, 0.2, 0.2, 0.0);
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");

	glGenBuffers(1, &vertPostionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);

	GLfloat sqVert[12] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,

		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, 12* sizeof(GLfloat), sqVert, GL_STATIC_DRAW);

	glGenBuffers(1, &vertColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
	GLfloat sqColors[24] = {
		1.0f, 0.7f, 0.3f, 1.0f,
		0.7f, 0.3f, 1.0f, 1.0f,
		0.3f, 1.0f, 0.7f, 1.0f,
		0.5f, 0.9f, 0.2f, 1.0f,
		0.9f, 0.2f, 0.5f, 1.0f,
		0.2f, 0.5f, 0.9f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqColors, GL_STATIC_DRAW);

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CS - 6533");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}