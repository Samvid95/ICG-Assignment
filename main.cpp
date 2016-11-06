#include "glsupport.h"
#include <glut.h>
#include "matrix4.h"
#include <iostream>
#include "quat.h"
#include <vector>
#include "geometrymaker.h"
#include <string>
#include <cassert>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

GLint program;
GLuint vertPostionVBO;
GLuint normalPositionVBO;
GLuint positionAttribute;

GLuint vertColorVBO;
GLuint colorAttribute;

GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint positionUniform;

GLuint normalAttribute;
GLuint normalMatrixUniformLocation;
GLuint uColorLocation;

GLuint vertexBO1, vertexBO2, vertexBO3;
GLuint indexBO1, indexBO2, indexBO3;

typedef struct Entity Entity;



struct VertexPN {
	Cvec3f p;
	Cvec3f n;
	
	VertexPN() {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}

	VertexPN& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		return *this;
	}
};

struct Geometry {
	void Draw(string type) {
	if (type == "Sphere") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO1);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO1);
	}
	if (type == "Plane") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO2);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO2);
	}
	if (type == "Cube") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO3);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO3);

	}

		glDrawElements(GL_TRIANGLES, sizeof(VertexPN) * 120, GL_UNSIGNED_SHORT, 0);
	}
};

class Entity {
private:
	Quat q1, q2, q3;
	Quat combined;

public:
	Geometry geometry;
	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Entity *parent;
	Matrix4 modelMatrix;

	Entity() {
		s = (1.0, 1.0, 1.0);
	}


	void createMatrix() {

		q1 = Quat::makeXRotation(r[0]);
		q2 = Quat::makeYRotation(r[1]);
		q3 = Quat::makeZRotation(r[2]);

		combined = q1 * q2 * q3;

		Matrix4 tempRMatrix;
		tempRMatrix = quatToMatrix(combined);

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
	
	void Draw(Matrix4 eyeMatrix, Matrix4 projectionMatrix, GLuint modelViewMatrixUniformLocation, GLuint projectionMatrixUniformLocation, GLuint normalMatrixUniformLocation, string type)
	{
		createMatrix();
		
		Matrix4 modelViewMatrix = inv(eyeMatrix) * modelMatrix;
		GLfloat glmatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
		glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

		GLfloat glmatrixProjection[16];
		projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

		Matrix4 normalMatrixtemp = normalMatrix(modelMatrix);
		GLfloat glMatrixANormal[16];
		normalMatrixtemp.writeToColumnMajorMatrix(glMatrixANormal);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glMatrixANormal);
		glUniform3f(uColorLocation, 1.0, 0.0, 0.0);

		geometry.Draw(type);
	}
};



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	int timeStart = glutGet(GLUT_ELAPSED_TIME);
	
	//EyeMatrix
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 35.0));

	//Projection Matrix
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);

	Entity matrixA;
	matrixA.t = Cvec3(0.0, 0.0, 0.0);
	matrixA.r = Cvec3(0.0, 45.0 * (float)timeStart / 1000.0f, 45.0 * (float)timeStart / 1000.0f);
	matrixA.s = Cvec3(1.0, 1.0, 1.0);
	matrixA.parent = NULL;
	matrixA.Draw(eyeMatrix, projectionMatrix, modelViewMatrixUniformLocation, projectionMatrixUniformLocation, normalMatrixUniformLocation, "Cube");
	
	Entity objectB;
	objectB.t = Cvec3(2.0, 2.0, 0.0);
	objectB.parent = &matrixA;
	objectB.Draw(eyeMatrix, projectionMatrix, modelViewMatrixUniformLocation, projectionMatrixUniformLocation, normalMatrixUniformLocation, "Cube");

	Entity objectC;
	objectC.t = Cvec3(-3.0, 4.0, 0.0);
	objectC.parent = NULL;
	objectC.Draw(eyeMatrix, projectionMatrix, modelViewMatrixUniformLocation, projectionMatrixUniformLocation, normalMatrixUniformLocation, "Sphere");
	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);

	glutSwapBuffers();
}

void SphereGenerator() {
	int ibLen, vbLen;
	getSphereVbIbLen(8, 8, vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makeSphere(2, 8, 8, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void PlaneGenerator() {
	int ibLen, vbLen;
	getPlaneVbIbLen(vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makePlane(4, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void CubeGenerator() {
	int ibLen, vbLen;
	getCubeVbIbLen(vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makeCube(3, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
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
	uColorLocation = glGetUniformLocation(program, "uColor");

	normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");
	normalAttribute = glGetAttribLocation(program, "normal");

	CubeGenerator();
	SphereGenerator();
	PlaneGenerator();


}

static void PrintInfo(const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::shape_t>& shapes,
	const std::vector<tinyobj::material_t>& materials) {
	std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
	std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
	
	for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			static_cast<const double>(attrib.vertices[3 * v + 0]),
			static_cast<const double>(attrib.vertices[3 * v + 1]),
			static_cast<const double>(attrib.vertices[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
		printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
			static_cast<const double>(attrib.normals[3 * v + 0]),
			static_cast<const double>(attrib.normals[3 * v + 1]),
			static_cast<const double>(attrib.normals[3 * v + 2]));
	}

}


static bool TestLoadObj(const char* filename, const char* basepath = NULL,
	bool triangulate = true) {
	std::cout << "Loading " << filename << std::endl;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename,
		basepath, triangulate);


	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		printf("Failed to load/parse .obj.\n");
		return false;
	}

	PrintInfo(attrib, shapes, materials);

	return true;
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

	cout << "Coming overhere!!";
	assert(true == TestLoadObj("lucy.obj", "/", false));
	init();
	glutMainLoop();
	

	return 0;
}