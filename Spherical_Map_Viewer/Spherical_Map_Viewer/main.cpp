#define _CRT_SECURE_NO_WARNINGS
#include <vgl.h>
#include <InitShader.h>
#include "MySphere.h"
#include "Targa.h"
#include "MyObject.h"
#include <string>

#include <vec.h>
#include <mat.h>

MySphere sphere;
MyObject object;

GLuint phong_prog;
GLuint spherical_prog;

mat4 g_Mat = mat4(1.0f);

float time = 0;
string fname;
float g_aspect = 1;
bool pause = true;
float zoom = 0;
float rotate_x=0;
float rotate_y=0;
int mode = 0;
int obj = 0;

GLint prev_x;
GLint prev_y;
GLint cur_x;
GLint cur_y;

//Fresnel Shading
float fP = 11.0;

//diffuseTexture
bool turnDiff = false;

mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	mat4 V = mat4(1.0f);

	up = normalize(up);
	vec3 n = normalize(at - eye);
	float a = dot(up, n);
	vec3 v = normalize(up - a * n);
	vec3 w = cross(n, v);

	V[0] = vec4(w, dot(-w, eye));
	V[1] = vec4(v, dot(-v, eye));
	V[2] = vec4(-n, dot(n, eye));

	return V;
}

mat4 myPerspective(float angle, float aspect, float zNear, float zFar)
{
	float rad = angle * 3.141592 / 180.0f;
	mat4 V(1.0f);
	float h = 2 * zFar*tan(rad / 2);
	float w = aspect * h;
	mat4 S = Scale(2 / w, 2 / h, 1 / zFar);

	float c = -zNear / zFar;

	mat4 Mpt(1.0f);
	Mpt[2] = vec4(0, 0, 1 / (c + 1), -c / (c + 1));
	Mpt[3] = vec4(0, 0, -1, 0);


	V = Mpt * S;

	return V;

}

void mySphericalMapInit() {
	STGA img;

	GLuint tex[2];
	glGenTextures(2, tex);

	string tname[2] = { fname + "_spheremap.tga",fname + "_diffusemap.tga" };
	
	for(int i=0; i<2; i++)
	{
	img.loadTGA(tname[i].c_str());
	switch(i)
	{
	case 0:
	glActiveTexture(GL_TEXTURE0);
	break;
	case 1:
	glActiveTexture(GL_TEXTURE1);
	break;
	default:
	break;
	}
	glBindTexture(GL_TEXTURE_2D, tex[i]);
	//mipmapping
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	img.width, img.height, 0, GL_BGR,
	GL_UNSIGNED_BYTE,img.data);

	glTexParameteri(GL_TEXTURE_2D,
	GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
	GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,
	GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,
	GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
}

void myInit()
{
	sphere.Init(30, 30);
	object.Init("bunny.obj");

	phong_prog = InitShader("vPhong.glsl", "fPhong.glsl");
	glUseProgram(phong_prog);

	spherical_prog = InitShader("vspherical.glsl", "fspherical.glsl");
	glUseProgram(spherical_prog);

	mySphericalMapInit();
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mat4 ModelMat = RotateY(time);

	mat4 CameraMat = RotateY(rotate_y)*RotateX(rotate_x);
	vec4 ePos = CameraMat*vec4(0, 0, 6 + zoom, 1);
	vec3 ePos3;
	ePos3.x = ePos.x;
	ePos3.y = ePos.y;
	ePos3.z = ePos.z;
	mat4 ViewMat = myLookAt(ePos3, vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 ProjMat = myPerspective(45, g_aspect, 0.01, 50.0f);


	glUseProgram(spherical_prog);
	GLuint uCamera = glGetUniformLocation(spherical_prog, "uCamera");
	glUniformMatrix4fv(uCamera, 1, GL_TRUE, CameraMat);
	GLuint uMat = glGetUniformLocation(spherical_prog, "uMat");
	glUniformMatrix4fv(uMat, 1, GL_TRUE, ProjMat*ViewMat*Scale(10, 10, 10));
	GLuint uTexture = glGetUniformLocation(spherical_prog, "uTexture");
	glUniform1i(uTexture, 0);

	sphere.Draw(spherical_prog);
	
	glUseProgram(phong_prog);

	// 1. Define Light Properties
	// 
	vec4 lPos = vec4(2, 2, 0, 1);
	vec4 lAmb = vec4(0.5, 0.5, 0.5, 1);
	vec4 lDif = vec4(1, 1, 1, 1);
	vec4 lSpc = lDif;

	// 2. Define Material Properties
	//
	vec4 mAmb = vec4(0.3, 0.3, 0.3, 1);
	vec4 mDif = vec4(0.7, 0.7, 0.7, 1);
	vec4 mSpc = vec4(0.3, 0.3, 0.3, 1);
	float mShiny = 50;										//1~100;

															// I = lAmb*mAmb + lDif*mDif*(N·L) + lSpc*mSpc*(V·R)^n; 
	vec4 amb = lAmb * mAmb;
	vec4 dif = lDif * mDif;
	vec4 spc = lSpc * mSpc;

	// 3. Send Uniform Variables to the shader
	//
	GLuint uModelMat = glGetUniformLocation(phong_prog, "uModelMat");
	GLuint uViewMat = glGetUniformLocation(phong_prog, "uViewMat");
	GLuint uProjMat = glGetUniformLocation(phong_prog, "uProjMat");
	GLuint uLPos = glGetUniformLocation(phong_prog, "uLPos");
	GLuint uAmb = glGetUniformLocation(phong_prog, "uAmb");
	GLuint uDif = glGetUniformLocation(phong_prog, "uDif");
	//GLuint uSpc = glGetUniformLocation(phong_prog, "uSpc");
	GLuint uEPos = glGetUniformLocation(phong_prog, "uEPos");
	GLuint uShininess = glGetUniformLocation(phong_prog, "uShininess");
	uTexture = glGetUniformLocation(phong_prog, "uTexture");
	GLuint uTexDiff = glGetUniformLocation(phong_prog, "uTexDiff");
	GLuint uFresnelPower = glGetUniformLocation(phong_prog, "uFresnelPower");

	glUniformMatrix4fv(uModelMat, 1, true, ModelMat);
	glUniformMatrix4fv(uViewMat, 1, true, ViewMat);
	glUniformMatrix4fv(uProjMat, 1, true, ProjMat);
	glUniform4f(uLPos, lPos[0], lPos[1], lPos[2], lPos[3]);
	glUniform4f(uAmb, amb[0], amb[1], amb[2], amb[3]);
	glUniform4f(uDif, dif[0], dif[1], dif[2], dif[3]);
	//glUniform4f(uSpc, spc[0], spc[1], spc[2], spc[3]);
	glUniform4f(uEPos, ePos[0], ePos[1], ePos[2], 1);
	glUniform1f(uShininess, mShiny);
	glUniform1i(uTexture, 0);
	glUniform1f(uFresnelPower, fP);

	if(turnDiff) glUniform1i(uTexDiff, 1);
	else glUniform1i(uTexDiff, 0);

	if(obj == 1)
		sphere.Draw(phong_prog);
	else if(obj == 2)
		object.Draw(phong_prog);
	
	glutSwapBuffers();
}


void idle()
{
	time += 1*pause;
	Sleep(33);					// for vSync
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	g_aspect = w / float(h);
	glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if (fP > 0.5)	fP-=0.5;
		printf("Fresnel Power : %f\n", fP);
		break;
	case '2':
		if(fP < 11.0)	fP+=0.5;
		printf("Fresnel Power : %f\n", fP);
		break;
	case '3':
		turnDiff = !turnDiff;
		if(turnDiff)printf("Diffuse Light Map On\n", turnDiff);
		else printf("Diffuse Light Map Off\n", turnDiff);
		break;

	case 'q':
	case 'Q':
		obj = (obj + 1) % 3;
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
		if (state == GLUT_DOWN) {
			mode = 1;
			prev_x = x;
			prev_y = y;
		}
		else if (state == GLUT_UP) {
			mode = 0;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			prev_y = y;
			mode = 2;
		}
		else if (state == GLUT_UP) {
			mode = 0;
		}

		break;
	default :
		break;
	}
}

void myMouseMove(GLint X, GLint Y) {
	
	cur_x = X;
	cur_y = Y;
	if (mode == 1) {
		rotate_y += (cur_x - prev_x) / 10.0;
		rotate_x += (cur_y - prev_y) / 10.0;

		if(abs(rotate_x) > 90) rotate_x -= (cur_y - prev_y) / 10.0;
	}
	if (mode == 2) {
		zoom -= (cur_y - prev_y) / 100.0;
		if (zoom < -3) zoom += (cur_y - prev_y) / 100.0;
		else if(zoom > 20) zoom += (cur_y - prev_y) / 100.0;	////fragmentShader에서 앞부분 표시 x하면 오목한 안쪽만 나오지 않을까??
	}
	//printf("%d %d\n", prev_y, cur_y);
	prev_x = cur_x;
	prev_y = cur_y;

}

int main(int argc, char ** argv)
{
	printf("texture의 이름을 입력해주세요 : ");

	cin>> fname;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(768, 512);

	glutCreateWindow("Spherical Map");

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
	glutMotionFunc(myMouseMove);
	glutMainLoop();

	return 0;
}
