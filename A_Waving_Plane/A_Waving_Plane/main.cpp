#define _CRT_SECURE_NO_WARNINGS		// fopen fopen_s

#include <vgl.h>
#include "MyPlain.h"

float time = 0;
bool pause = false;
MyPlain plain;

void myInit()
{
	plain.initPlain(30,30);
	printf("div : %d	", plain.divM);
	printf("Number of Triangle : %d	", plain.divN*plain.divM * 2);
	printf("Number of Vertices : %d\n", plain.divN*plain.divM * 2 * 3);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	plain.draw(time);

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
		plain.divN--;
		plain.divM--;
		if (plain.divN < 2) plain.divN = 2;
		if (plain.divM < 2) plain.divM = 2;
		plain.initPlain(plain.divN, plain.divM);
		printf("div : %d	", plain.divM);
		printf("Number of Triangle : %d	", plain.divN*plain.divM * 2);
		printf("Number of Vertices : %d\n", plain.divN*plain.divM * 2 * 3);
		break;
	case '2':
		plain.divN++;
		plain.divM++;
		plain.initPlain(plain.divN, plain.divM);
		printf("div : %d	", plain.divM);
		printf("Number of Triangle : %d	", plain.divN*plain.divM * 2);
		printf("Number of Vertices : %d\n", plain.divN*plain.divM * 2 * 3);
		break;
	case 'w':
	case 'W':
		if (plain.div == 0)
			plain.div = plain.divM / 2;
		else
			plain.div = 0;
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
	printf("A Waving Color Plain\nProgramming Assignment #2 for Computer Graphics 2\nDepartment of Digital Contents, Sejong University\n");
	printf("\n----------------------------------------------------------------\n");
	printf("Spacebar: starting/stoping rotation\n");
	printf("`1' key : Decreaing the Number of Edges\n");
	printf("`2' key : Increaing the Number of Edges\n");
	printf("`w' key : Showing/hiding the waving pattern\n");
	printf("`Q' Key : Exit the program.");
	printf("\n----------------------------------------------------------------\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Waving Plain");

	glewExperimental = true;
	glewInit();

	myInit();

	glutIdleFunc(myIdle);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMainLoop();

	return 0;
}