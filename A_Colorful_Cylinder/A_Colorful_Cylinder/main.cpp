#define _CRT_SECURE_NO_WARNINGS		// fopen fopen_s

#include <vgl.h>
#include "MyColorCylinder.h"

float time = 0;
int angle = 10;
bool pause = false;
MyColorCylinder cylinder;

void myInit()
{
	cylinder.initCylinder(angle);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	cylinder.draw(time, angle);

	glutSwapBuffers();
}

void myIdle()
{
	time += 0.033 *pause;
	Sleep(33);
	glutPostRedisplay();

}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if(angle > 3)
			angle--;
		printf("Polygon : %d	", angle);
		printf("Number of Triangle : %d	", angle * 4);
		printf("Number of Vertices : %d\n", angle * 4 * 3);
		break;
	case '2':
		angle++;
		printf("Polygon : %d	", angle);
		printf("Number of Triangle : %d	", angle * 4);
		printf("Number of Vertices : %d\n", angle * 4 * 3);
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

int main(int argc, char ** argv)
{
	printf("A Rotating Color Cylinder\nProgramming Assignment #1 for Computer Graphics 2\nDepartment of Digital Contents, Sejong University\n");
	printf("\n----------------------------------------------------------------");
	printf("Spacebar: starting/stoping rotation\n");
	printf("`1' key: Decreaing the Number of Edges\n");
	printf("`2' key : Increaing the Number of Edges\n");
	printf("`Q' Key: Exit the program.");
	printf("\n----------------------------------------------------------------\n");
	printf("Polygon : %d	", angle);
	printf("Number of Triangle : %d	", angle * 4);
	printf("Number of Vertices : %d\n", angle * 4 * 3);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Color Cube");

	glewExperimental = true;
	glewInit();

	myInit();

	glutIdleFunc(myIdle);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();

	return 0;
}