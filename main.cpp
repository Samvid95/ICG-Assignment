#include "glsupport.h"
#include <glut.h>
#include "matrix4.h"
#include <iostream>

using namespace std;

GLint program;
GLuint vertPostionVBO;
GLuint positionAttribute;

GLuint vertColorVBO;
GLuint colorAttribute;

GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint positionUniform;



typedef struct Entity Entity;

struct Entity {
	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Entity *parent;
	Matrix4 modelMatrix;

	Entity() : s(1.0, 1.0, 1.0) {};
	
	 
	void createMatrix() {
		
		Matrix4 tempRMatrix;
		tempRMatrix = tempRMatrix.makeXRotation(r[0]);

		Matrix4 tempTMatrix;
		tempTMatrix = tempTMatrix.makeTranslation(t);

		Matrix4 tempSMatrix;
		tempSMatrix = tempSMatrix.makeScale(s);		
		if (this->parent == NULL) {
			modelMatrix = tempTMatrix * tempRMatrix * tempSMatrix;
		}
		else {
			modelMatrix = parent->modelMatrix * tempTMatrix * tempRMatrix * tempSMatrix;
		}

		
	}
};

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	int timeStart = glutGet(GLUT_ELAPSED_TIME);

	//Defining objectA
	Entity matrixA;
	matrixA.t = Cvec3(2.0, 0.0, 0.0);
	matrixA.r = Cvec3(45.0 * (float)timeStart/1000.0f, 0.0, 0.0);
	matrixA.s = Cvec3(5.0, 0.5, 1.0);
	matrixA.parent = NULL;
	matrixA.createMatrix();
	
	//Defining objectB
	Entity matrixB;
	matrixB.t = Cvec3(0.5, 3.0, 0.0);
	matrixB.r = Cvec3(45.0 * (float)timeStart/1000.0f, 0.0, 0.0);
	matrixB.s = Cvec3(0.5, 5.0, 1.0);
	matrixB.parent = &matrixA;
	matrixB.createMatrix();

	//Defining objectB
	Entity matrixC;
	matrixC.t = Cvec3(-1.5, -6.5, 3.0);
	matrixC.r = Cvec3(5* (float)timeStart / 1000.0f, 0.0, 0.0);
	matrixC.s = Cvec3(1.0, 1.0, 1.0);
	matrixC.parent = &matrixB;
	matrixC.createMatrix();


	//EyeMatrix 
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 20.0));
	
	//Projection Matrix
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);

	//Rendering MatrixA
	Matrix4 modelViewAMatrix = inv(eyeMatrix) * matrixA.modelMatrix;
	GLfloat glmatrixA[16];
	modelViewAMatrix.writeToColumnMajorMatrix(glmatrixA);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrixA);
	
	GLfloat glmatrixAProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixAProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixAProjection);
	glUniform4f(positionUniform, 0.0, 0.0, 0.00f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	//Rendering MatrixB
	Matrix4 modelViewBMatrix = inv(eyeMatrix) * matrixB.modelMatrix;
	GLfloat glMatrixB[16];
	modelViewBMatrix.writeToColumnMajorMatrix(glMatrixB);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glMatrixB);

	GLfloat glMatrixBProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glMatrixBProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glMatrixBProjection);
	glUniform4f(positionUniform, 0.0, 0.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	//Rendering MatrixB
	Matrix4 modelViewCMatrix = inv(eyeMatrix) * matrixC.modelMatrix;
	GLfloat glMatrixC[16];
	modelViewCMatrix.writeToColumnMajorMatrix(glMatrixC);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glMatrixC);

	GLfloat glMatrixCProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glMatrixCProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glMatrixCProjection);
	glUniform4f(positionUniform, 0.0, 0.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	
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
	positionUniform = glGetUniformLocation(program, "modelPosition");


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

	GLfloat cubeColors[] = {
		0.583f, 0.771f, 0.014f, 1.0f,
		0.609f, 0.115f, 0.436f, 1.0f,
		0.327f, 0.483f, 0.844f, 1.0f,
		0.822f, 0.569f, 0.201f, 1.0f,
		0.435f, 0.602f, 0.223f, 1.0f,
		0.310f, 0.747f, 0.185f, 1.0f,
		0.597f, 0.770f, 0.761f, 1.0f,
		0.559f, 0.436f, 0.730f, 1.0f,
		0.359f, 0.583f, 0.152f, 1.0f,
		0.483f, 0.596f, 0.789f, 1.0f,
		0.559f, 0.861f, 0.639f, 1.0f,
		0.195f, 0.548f, 0.859f, 1.0f,
		0.014f, 0.184f, 0.576f, 1.0f,
		0.771f, 0.328f, 0.970f, 1.0f,
		0.406f, 0.615f, 0.116f, 1.0f,
		0.676f, 0.977f, 0.133f, 1.0f,
		0.971f, 0.572f, 0.833f, 1.0f,
		0.140f, 0.616f, 0.489f, 1.0f,
		0.997f, 0.513f, 0.064f, 1.0f,
		0.945f, 0.719f, 0.592f, 1.0f,
		0.543f, 0.021f, 0.978f, 1.0f,
		0.279f, 0.317f, 0.505f, 1.0f,
		0.167f, 0.620f, 0.077f, 1.0f,
		0.347f, 0.857f, 0.137f, 1.0f,
		0.055f, 0.953f, 0.042f, 1.0f,
		0.714f, 0.505f, 0.345f, 1.0f,
		0.783f, 0.290f, 0.734f, 1.0f,
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
	glutCreateWindow("CS - 6533");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}