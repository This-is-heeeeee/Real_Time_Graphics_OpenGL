#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include <stack>
#include <math.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

using namespace std;

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;

bool bPlay = false;
bool bChasingTarget= false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

vec4 t_pos;

void myInit()
{
	cube.Init();
	pyramid.Init();	

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float time = 0;

void drawRobotArm(float ang1, float ang2, float ang3)
{
	stack<mat4> stc;

	// BASE
	mat4 M(1.0);

	/*M으로 모양 만들고 CTM으로 위치등을 조정한다.*/

	//upper Arm
	CTM = Translate(0,-0.3,0)*RotateY(time * 30)*RotateZ(ang1)*Translate(0,0.2,0);
	M = Scale(0.1, 0.5, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	stc.push(CTM);
	
	//처음에 upper Arm부터 그렸기(원점에 upper Arm을 그렸기) 때문에 shoulder를 회전축이 되게 하기 위해서는 upper Arm의 좌표를 위로 올렸다가 다시 내려야 한다.
	CTM *= Translate(0, -0.2, 0) * RotateZ(-ang1) * Translate(0,0.2,0);
	stc.push(CTM);

	//shoulder1
	CTM *= Translate(0, -0.2, 0.075);
	M = Scale(0.3, 0.3, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	//shoulder joint1
	CTM *= Translate(0,0,0.02);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = stc.top();
	stc.pop();
	
	//shoulder2
	CTM *= Translate(0, -0.2, -0.075);
	M = Scale(0.3, 0.3, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	//shoulder joint2
	CTM *= Translate(0, 0, -0.02);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = stc.top();

	//Arm joint1
	CTM *= Translate(0, 0.2, 0.095);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//lower Arm1
	CTM *= RotateZ(ang2)*Translate(0, 0.2, -0.02);
	M = Scale(0.1, 0.5, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//wrist1
	CTM *= Translate(0, 0.2, 0.02);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = stc.top();

	//Arm joint2
	CTM *= Translate(0, 0.2, -0.095);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//lower Arm2
	CTM *= RotateZ(ang2)*Translate(0, 0.2, 0.02);
	M = Scale(0.1, 0.5, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//wrist2
	CTM *= Translate(0, 0.2, -0.02);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//hand
	CTM *= RotateZ(ang3)*Translate(0, 0, 0.095);
	M = Scale(0.35, 0.15, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

}

void computeAngle()
{
	t_pos = target.GetPosition(time);

	vec4 h_pos = RotateZ(ang1)*Translate(0, 0.4, -0.095)*RotateZ(ang2)*Translate(0, 0.4, 0)*RotateZ(ang3)*Translate(0, 0, 0.095)*vec3(0.15, 0, 0);
	if (sqrt((h_pos.x - t_pos.x)*(h_pos.x - t_pos.x) + (h_pos.y - t_pos.y)*(h_pos.y - t_pos.y)) < 0.05) return;
	
	mat4 CTM1 = RotateZ(ang1+5)*Translate(0, 0.4, -0.095)*RotateZ(ang2)*Translate(0, 0.4, 0)*RotateZ(ang3)*Translate(0, 0, 0.095);
	mat4 CTM2 = RotateZ(ang1-5)*Translate(0, 0.4, -0.095)*RotateZ(ang2)*Translate(0, 0.4, 0)*RotateZ(ang3)*Translate(0, 0, 0.095);
	vec4 h_pos1 = CTM1 * vec3(0.175, 0, 0);
	vec4 h_pos2 = CTM2 * vec3(0.175, 0, 0);
	
	float dist1 = sqrt((h_pos1.x - t_pos.x)*(h_pos1.x - t_pos.x)+ (h_pos1.y - t_pos.y)*(h_pos1.y - t_pos.y));
	float dist2 = sqrt((h_pos2.x - t_pos.x)*(h_pos2.x - t_pos.x) + (h_pos2.y - t_pos.y)*(h_pos2.y - t_pos.y));

	ang1 = (dist1 < dist2) ? (ang1 + 5) : (ang1 - 5);

	CTM1 = RotateZ(ang1)*Translate(0, 0.4, -0.095)*RotateZ(ang2+5)*Translate(0, 0.4, 0)*RotateZ(ang3)*Translate(0, 0, 0.095);
	CTM2 = RotateZ(ang1)*Translate(0, 0.4, -0.095)*RotateZ(ang2-5)*Translate(0, 0.4, 0)*RotateZ(ang3)*Translate(0, 0, 0.095);
	h_pos1 = CTM1 * vec3(0.175, 0, 0);
	h_pos2 = CTM2 * vec3(0.175, 0, 0);

	dist1 = sqrt((h_pos1.x - t_pos.x)*(h_pos1.x - t_pos.x) + (h_pos1.y - t_pos.y)*(h_pos1.y - t_pos.y));
	dist2 = sqrt((h_pos2.x - t_pos.x)*(h_pos2.x - t_pos.x) + (h_pos2.y - t_pos.y)*(h_pos2.y - t_pos.y));

	ang2 = (dist1 < dist2) ? (ang2 + 5) : (ang2 - 5);

	CTM1 = RotateZ(ang1)*Translate(0, 0.4, -0.095)*RotateZ(ang2)*Translate(0, 0.4, 0)*RotateZ(ang3+1)*Translate(0, 0, 0.095);
	CTM2 = RotateZ(ang1)*Translate(0, 0.4, -0.095)*RotateZ(ang2)*Translate(0, 0.4, 0)*RotateZ(ang3-1)*Translate(0, 0, 0.095);
	h_pos1 = CTM1 * vec3(0.175, 0, 0);
	h_pos2 = CTM2 * vec3(0.175, 0, 0);

	dist1 = sqrt((h_pos1.x - t_pos.x)*(h_pos1.x - t_pos.x) + (h_pos1.y - t_pos.y)*(h_pos1.y - t_pos.y));
	dist2 = sqrt((h_pos2.x - t_pos.x)*(h_pos2.x - t_pos.x) + (h_pos2.y - t_pos.y)*(h_pos2.y - t_pos.y));

	ang3 = (dist1 < dist2) ? (ang3 + 1) : (ang3 - 1);
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1,-1,-1,-1);

	
	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(time*30);
	drawRobotArm(ang1, ang2, ang3);	
	

	glUniform4f(uColor, 1,0,0,1);
	if (bDrawTarget == true) {
		CTM = Translate(0, -0.3, 0)*RotateY(time * 30);
		target.Draw(program, CTM, time);
	}

	glutSwapBuffers();
}

void myIdle()
{
	if(bPlay)
	{
		time += 1/60.0f;
		Sleep(1/60.0f*1000);

		if(bChasingTarget == false)
		{
			ang1 = 45 * sin(time*3.141592);
			ang2 = 60 * sin(time*2*3.141592);
			ang3 = 30 * sin(time*3.141592);
		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch(c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Simple Robot Arm");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}