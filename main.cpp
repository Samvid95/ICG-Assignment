#include "glsupport.h"
#include <glut.h>


GLint program;
GLuint vertPostionVBO;
GLuint positionAttribute;

GLuint vertTexCoordVBO;
GLuint texCoordAttribute;

GLuint smurfTexture;
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);

	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, smurfTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(texCoordAttribute);

	glutSwapBuffers();
}

void init() {
	glClearColor(0.2, 0.2, 0.2, 0.0);
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	smurfTexture = loadGLTexture("Smurf1.png");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	texCoordAttribute = glGetAttribLocation(program, "texCoord");

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

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqVert, GL_STATIC_DRAW);

	glGenBuffers(1, &vertTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	GLfloat sqTexCoords[12] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqTexCoords, GL_STATIC_DRAW);

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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