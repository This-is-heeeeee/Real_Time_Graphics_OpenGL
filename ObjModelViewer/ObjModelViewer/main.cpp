#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>
#include "MyCube.h"
#include "MyObj.h"

#include <vec.h>
#include <mat.h>

MyCube cube;
MyObj Obj;

GLuint program;
GLuint prog_phong;

GLuint uMat;
GLuint uColor;
mat4 g_Mat = mat4(1.0f);
float gloss = 100;
float ks = 1.0;

int winWidth = 500;
int winHeight = 500;

float time = 0;
bool pause = false;
bool mode = true;
int rotate = 0;
mat4 ModelMat;

//obj
GLuint vertexNum = 0;
GLuint faceNum = 0;
vec3 *ver;
vec3 *face;
GLfloat avgX;
GLfloat avgY;
GLfloat avgZ;
GLfloat scaleAll;
int N = 0;

mat4 x_a = Translate(1, 0, 0)*Scale(2, 0.02, 0.02);
mat4 y_a = Translate(0, 1, 0)*Scale(0.02, 2, 0.02);
mat4 z_a = Translate(0, 0, 1)*Scale(0.02, 0.02, 2);

mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	// Implement your own look-at function
	mat4 V(1.0f);
	vec3 n = at - eye;
	n /= length(n);

	float a = dot(up, n);
	vec3 v = up - a * n;
	v /= length(v);

	vec3 w = cross(n, v);

	mat4 Rw(1.0f);

	Rw[0][0] = w.x;	Rw[0][1] = v.x; Rw[0][2] = -n.x;
	Rw[1][0] = w.y;	Rw[1][1] = v.y; Rw[1][2] = -n.y;
	Rw[2][0] = w.z;	Rw[2][1] = v.z; Rw[2][2] = -n.z;

	mat4 Rc(1.0f);
	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			Rc[i][j] = Rw[j][i];

	mat4 Tc = Translate(-eye.x, -eye.y, -eye.z);

	V = Rc * Tc;

	return V;
}

mat4 myPerspective(float fovy, float aspectRatio, float zNear, float zFar)
{
	mat4 P(1.0f);

	float rad = fovy * 3.141592 / 180.0;

	float sz = 1 / zFar;
	float h = zFar * tan(rad / 2);

	float sy = 1 / h;
	float w = h * aspectRatio;
	float sx = 1 / w;

	mat4 S = Scale(sx, sy, sz);
	mat4 M(1.0f);

	float c = -zNear / zFar;
	M[2][2] = 1 / (c + 1);
	M[2][3] = -c / (c + 1);
	M[3][2] = -1;
	M[3][3] = 0;

	P = M * S;

	return P;
}


void myInit()
{
	cube.Init();
	Obj.Init(vertexNum, faceNum, ver, face, vec4(0.7,0.5,0.7,1), mode);

	program = InitShader("vshader.glsl", "fshader.glsl");
	prog_phong = InitShader("vphong.glsl", "fphong.glsl");
}

void matInit() {
	if (rotate == 0) {
		x_a = RotateX(time) * x_a;
		y_a = RotateX(time) * y_a;
		z_a = RotateX(time) * z_a;
		ModelMat = RotateX(time) * ModelMat;

	}
	else if(rotate == 1) {
		x_a = RotateY(time) * x_a;
		y_a = RotateY(time) * y_a;
		z_a = RotateY(time) * z_a;
		ModelMat = RotateY(time) * ModelMat;

	}
	else {
		x_a = RotateZ(time) * x_a;
		y_a = RotateZ(time) * y_a;
		z_a = RotateZ(time) * z_a;
		ModelMat = RotateZ(time) * ModelMat;

	}
}

void DrawAxis()
{
	matInit();

	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat*x_a);
	glUniform4f(uColor, 1, 0, 0, 1);
	cube.Draw(program);

	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat*y_a);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat*z_a);
	glUniform4f(uColor, 0, 0, 1, 1);
	cube.Draw(program);
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mat4 ViewMat = myLookAt(vec3(0, 0, 4), vec3(0, 0, 0), vec3(0, 1, 0));

	float aspect = winWidth / (float)winHeight;
	float h = 1;

	mat4 ProjMat = myPerspective(40, aspect, 0.1, 100);

	g_Mat = ProjMat * ViewMat;

	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");

	DrawAxis();

	glUseProgram(prog_phong);
	GLuint uProjMat = glGetUniformLocation(prog_phong, "uProjMat");
	GLuint uModelMat = glGetUniformLocation(prog_phong, "uModelMat");
	GLuint uGloss = glGetUniformLocation(prog_phong, "uGloss");
	GLuint uKs = glGetUniformLocation(prog_phong, "uKs");

	matInit();

	glUniformMatrix4fv(uProjMat, 1, GL_TRUE, ProjMat);
	glUniformMatrix4fv(uModelMat, 1, GL_TRUE, ViewMat *ModelMat);
	glUniform1f(uGloss, gloss);
	glUniform1f(uKs, ks);

	Obj.Draw(prog_phong);

	glutSwapBuffers();
}


void idle()
{
	time = 1 * pause;
	Sleep(16);					// for vSync
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

void loadObj(char *fname) {
	FILE *fp;
	char count[100];

	fp = fopen(fname, "r");

	while (!fp) {
		printf("Input File name : ");
		scanf("%s", fname);

		fp = fopen(fname, "r");

		if (!fp) {
			printf("File not found!!\n");
		}
	}

	while (!feof(fp)) {
		fscanf(fp, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '0', sizeof(count));
	}
	printf("vertexNum : %d ", vertexNum);
	printf("faceNum : %d\n", faceNum);

	fclose(fp);

	ver = new vec3[vertexNum];
	face = new vec3[faceNum];

	int read;
	char ch;
	float x;
	float y;
	float z;
	GLuint vertIdx = 0;
	GLuint faceIdx = 0;
	GLfloat maxX = -1.0;
	GLfloat maxY = -1.0;
	GLfloat maxZ = -1.0;
	GLfloat sumX = 0;
	GLfloat sumY = 0;
	GLfloat sumZ = 0;
	GLfloat scaleX;
	GLfloat scaleY;
	GLfloat scaleZ;

	fp = fopen(fname, "r");
	
	while (!feof(fp)) {
		read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
		if (read == 4 && ch == 'v') {
			ver[vertIdx].x = x;
			ver[vertIdx].y = y;
			ver[vertIdx].z = z;

			if (ver[vertIdx].x > maxX)	maxX = ver[vertIdx].x;
			if (ver[vertIdx].y > maxY)	maxY = ver[vertIdx].y;
			if (ver[vertIdx].z > maxZ)	maxZ = ver[vertIdx].z;

			sumX += ver[vertIdx].x;
			sumY += ver[vertIdx].y;
			sumZ += ver[vertIdx].z;

			vertIdx++;
		}
		else if (read == 4 && ch == 'f') {
			face[faceIdx].x = x - 1.0;
			face[faceIdx].y = y - 1.0;
			face[faceIdx].z = z - 1.0;
			faceIdx++;
		}
		avgX = sumX / vertexNum;
		avgY = sumY / vertexNum;
		avgZ = sumZ / vertexNum;

		scaleX = (1.0 - maxX) * 10 + 1;
		scaleY = (1.0 - maxY) * 10 + 1;
		scaleZ = (1.0 - maxZ) * 10 + 1;

		if (scaleX > scaleY) {
			if (scaleY > scaleZ)
				scaleAll = scaleZ;
			else
				scaleAll = scaleY;
		}
		else if (scaleX <scaleY) {
			if (scaleX < scaleZ)
				scaleAll = scaleX;
			else
				scaleAll = scaleZ;
		}
	}

	ModelMat = Scale(scaleAll) * Translate(-avgX, -avgY, -avgZ);

	fclose(fp);
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		mode = true;
		Obj.reShading(vertexNum, faceNum, ver, face, mode);

		break;
	case '2':
		mode = false;
		Obj.reShading(vertexNum, faceNum, ver, face, mode);
		break;
	case '3':
		if(ks <1.5)
			ks += 0.1;
		break;
	case '4':
		if(ks > 0.0)
			ks -= 0.1;
		break;
	case '5':
		if(gloss < 150)
			gloss += 10;
		break;
	case '6':
		if(gloss > 10)
			gloss -= 10;
		break;
	case 'q':
	case 'Q':
		exit(1);
		break;
	case 32:
		pause = !pause;
		break;
	}
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			rotate = 0;
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			rotate = 1;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			rotate = 2;
		}
		break;
	}
}

int main(int argc, char ** argv)
{
	char fname[100];// = "dragon.obj";

	loadObj(fname);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow("OpenGL");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMainLoop();

	return 0;
}