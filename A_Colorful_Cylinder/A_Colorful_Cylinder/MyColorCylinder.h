#pragma once

#include <vgl.h>
#include <InitShader.h>
#include <vec.h>

vec4 vertex_pos[2] = {
	vec4(0.0,0.0,0.5,1.0),
	vec4(0.0,0.0,-0.5,1.0)};

class MyColorCylinder
{
public:
	int ind = 0;
	int numVertices;
	int sizeofPoints;
	int sizeofColors;
	vec4 * points;
	vec4 * colors;
	GLuint vao;
	GLuint buffer;
	GLuint prog;

	MyColorCylinder()
	{
		numVertices = 10*4*3;
		points = NULL;
		colors = NULL;
	}
	MyColorCylinder(int n) {
		numVertices = n * 4 * 3;
		points = NULL;
		colors = NULL;
	}
	~MyColorCylinder()
	{
		delete[] points;
		delete[] colors;
	} 
	void makeUpTri(int a, int b)
	{
		float rad = 0.5;
		float theta = 2.0 / a * 3.141592;

		points[ind] = vec4(rad*sin(theta*(b + 1)), rad*cos(theta*(b + 1)), 0.5, 1.0); colors[ind] = vec4(1.0, 1.0, 0.0, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*b), rad*cos(theta*b), 0.5, 1.0); colors[ind] = vec4(1.0, 1.0, 0.0, 1.0); ind++;
		points[ind] = vertex_pos[0]; colors[ind] = vec4(1.0, 1.0, 0.0, 1.0); ind++;
	}

	void makeQuad(int a, int b) {
		float rad = 0.5;
		float theta = 2.0 / a * 3.141592;
		float r = 1.0 / (a - 1) * b;
		float blue = 1.0 - 1.0 / (a - 1) * b;
		float g = 0.0;
		
		points[ind] = vec4(rad*sin(theta*(b + 1)), rad*cos(theta*(b + 1)), 0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*(b + 1)), rad*cos(theta*(b + 1)), -0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*b), rad*cos(theta*b), -0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;

		points[ind] = vec4(rad*sin(theta*b), rad*cos(theta*b), -0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*b), rad*cos(theta*b), 0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*(b+1)), rad*cos(theta*(b+1)), 0.5, 1.0); colors[ind] = vec4(r, g, blue, 1.0); ind++;
	}

	void makeLowTri(int a, int b)
	{
		float rad = 0.5;
		float theta = 2.0 / a * 3.141592;

		points[ind] = vec4(rad*sin(theta*(b + 1)), rad*cos(theta*(b + 1)), -0.5, 1.0); colors[ind] = vec4(1.0, 1.0, 1.0, 1.0); ind++;
		points[ind] = vec4(rad*sin(theta*b), rad*cos(theta*b), -0.5, 1.0); colors[ind] = vec4(1.0, 1.0, 1.0, 1.0); ind++;
		points[ind] = vertex_pos[1]; colors[ind] = vec4(1.0, 1.0, 1.0, 1.0); ind++;
	}
	
	void makeCylinder(int n)
	{
		ind = 0;

		for (int i = 0; i < n; i++) {
			makeUpTri(n,i);
		}
		
		for (int i = 0; i < n; i++) {
			makeQuad(n, i);
		}
		
		for (int i = 0; i < n; i++) {
			makeLowTri(n, i);
		}
	}

	void allocation() {
		points = new vec4[numVertices];
		colors = new vec4[numVertices];

		sizeofPoints = sizeof(vec4)*numVertices;
		sizeofColors = sizeof(vec4)*numVertices;
	}

	void BufferData() {
		glBufferData(GL_ARRAY_BUFFER, sizeofPoints + sizeofColors,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			sizeofPoints, points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofPoints,
			sizeofColors, colors);
	}

	void linkShader() {
		GLuint vPosition = glGetAttribLocation(prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT,
			false, 0, BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT,
			false, 0, BUFFER_OFFSET(sizeofPoints));
	}

	void initCylinder(int n)
	{
		allocation();

		makeCylinder(n);
		// send data to GPU
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		
		BufferData();

		// upload shaders to GPU
		prog =
			InitShader("vshader.glsl", "fshader.glsl");
		glUseProgram(prog);
		
		// link the date to the vshader
		linkShader();
	}

	void draw(float time, int angle)
	{
		glBindVertexArray(vao);
		glUseProgram(prog);

		delete[] points;
		delete[] colors;

		numVertices = angle * 4 * 3;
		
		allocation();	//동적할당

		makeCylinder(angle);	//좌표 계산
	
		BufferData();	//버퍼 설정

		linkShader();	//vshader와 연결

		GLuint uTime = glGetUniformLocation(prog, "uTime");
		glUniform1f(uTime, time);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);

	}
};