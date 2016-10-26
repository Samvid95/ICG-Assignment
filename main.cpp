#include "glsupport.h"
#include <GL/freeglut.h>
#include "matrix4.h"
#include "quat.h"
#include <math.h>
#include <iostream>

using namespace std;

GLuint program;

GLuint vertexBufferVBO;
GLuint colorBufferVBO;

GLuint positionAttribute;
GLuint colorAttribute;

GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;

struct Entity
{
	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Matrix4 modelMatrix;
	Entity *parent;
	
	Entity() : s(1.0, 1.0, 1.0) {}

	void createMatrix() {
		Matrix4 rotationMatrix,scaleMatrix,tranlationMatrix;
		
		Quat rotationX = Quat::makeXRotation(r[0]);
		Quat rotationY = Quat::makeYRotation(r[1]);
		Quat rotationZ = Quat::makeZRotation(r[2]);
		Quat rotation = rotationX * rotationY * rotationZ;
		rotationMatrix = quatToMatrix(rotation);
		
		scaleMatrix = scaleMatrix.makeScale(s);

		tranlationMatrix = tranlationMatrix.makeTranslation(t);

		if (this->parent == NULL) {
			modelMatrix = tranlationMatrix * rotationMatrix * scaleMatrix;	
		}
		else {
			modelMatrix = parent->modelMatrix * tranlationMatrix * rotationMatrix * scaleMatrix;
		}
	}

	void drawObject(Matrix4 eyeMatrix, Matrix4 projectionMatrix) {
		Matrix4 modelViewMatrix = inv(eyeMatrix) * modelMatrix;
		GLfloat glmatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
		glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

		GLfloat glmatrixProjection[16];
		projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
};



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = glutGet(GLUT_ELAPSED_TIME);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 10.0));

	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);

	Entity objectA;
	objectA.r = Cvec3(45.0 * (float)time / 1000.0f,0.0, 0.0);
	objectA.t = Cvec3(0.0, 0.0, 0.0);
	objectA.parent = NULL;
	objectA.createMatrix();
	objectA.drawObject(eyeMatrix, projectionMatrix);

	Entity objectB;
	objectB.r = Cvec3(0.0, 30.0 * (float)time / 1000.0f, 0.0);
	objectB.t = Cvec3(-3.0, 0.0, 0.0);
	objectB.parent = &objectA;
	objectB.createMatrix();
	objectB.drawObject(eyeMatrix, projectionMatrix);

	Entity objectC;
	float sinevalue = sin((float)time / 1000.0);
	cout << sinevalue << endl;
	objectC.r = Cvec3(0.0,0.0, 10.0 * (float)time / 1000.0f);
	objectC.t = Cvec3(3.0, 0.0, 0.0);
	objectC.s = Cvec3(1.1 + sinevalue, 1.0, 1.0);
	objectC.parent = &objectB;
	objectC.createMatrix();
	objectC.drawObject(eyeMatrix, projectionMatrix);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	

	glutSwapBuffers();
}

void init() {
	glClearDepth(0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glReadBuffer(GL_BACK);


	glClearColor(0.2, 0.2, 0.2, 0.0);
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");
	
	modelViewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");


	glGenBuffers(1, &vertexBufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferVBO);

	GLfloat cubeVerts[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &colorBufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferVBO);

	GLfloat cubeColors[] = {
		1.583f, 0.771f, 0.014f, 1.0f,
		1.609f, 0.115f, 0.436f, 1.0f,
		0.327f, 0.483f, 0.844f, 1.0f,
		0.822f, 0.569f, 0.201f, 1.0f,
		0.435f, 0.602f, 0.223f, 1.0f,
		0.310f, 0.747f, 0.185f, 1.0f,
		0.597f, 0.770f, 0.761f, 1.0f,
		0.559f, 1.436f, 0.730f, 1.0f,
		0.359f, 0.583f, 0.152f, 1.0f,
		0.483f, 0.596f, 0.789f, 1.0f,
		0.559f, 0.861f, 0.639f, 1.0f,
		0.195f, 0.548f, 0.859f, 1.0f,
		1.014f, 0.184f, 0.576f, 1.0f,
		1.771f, 0.328f, 0.970f, 1.0f,
		1.406f, 0.615f, 0.116f, 1.0f,
		1.676f, 0.977f, 0.133f, 1.0f,
		1.971f, 0.572f, 0.833f, 1.0f,
		1.140f, 0.616f, 0.489f, 1.0f,
		1.997f, 0.513f, 0.064f, 1.0f,
		1.945f, 0.719f, 0.592f, 1.0f,
		1.543f, 0.021f, 0.978f, 1.0f,
		1.279f, 0.317f, 0.505f, 1.0f,
		1.167f, 0.620f, 0.077f, 1.0f,
		1.347f, 0.857f, 0.137f, 1.0f,
		1.055f, 0.953f, 0.042f, 1.0f,
		1.714f, 0.505f, 0.345f, 1.0f,
		1.783f, 0.290f, 0.734f, 1.0f,
		0.722f, 0.645f, 0.174f, 1.0f,
		0.302f, 0.455f, 0.848f, 1.0f,
		0.225f, 0.587f, 0.040f, 1.0f,
		0.517f, 0.713f, 0.338f, 1.0f,
		0.053f, 0.959f, 0.120f, 1.0f,
		0.393f, 0.621f, 0.362f, 1.0f,
		0.673f, 0.211f, 0.457f, 1.0f,
		0.820f, 0.883f, 0.371f, 1.0f,
		0.982f, 0.099f, 0.879f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);

}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CS-6533");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}