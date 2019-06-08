#pragma once
#include <vgl.h>
#include <InitShader.h>
#include <vec.h>

class MyPlain {
public:
	int numVertices;
	int divN;
	int divM;
	float div;
	
	vec4 *points;
	vec4 *colors;
	GLuint vao = 0;
	GLuint buffer = 0;
	GLuint prog = 0;

	MyPlain() {
		numVertices = divN = divM = div = 0;
		points = NULL;
		colors = NULL;
	}
	~MyPlain() {
		delete[] points;
		delete[] colors;
	}

	void makePlain() {
		int ind = 0;
		
		for (int i = 0; i < divN; i++) {
			for (int j = 0; j < divM; j++) {
				float x1 = (1.5/divN * i)-0.75;
				float y1 = (1.5/divM * j)-0.75;
				float x2 = (1.5/divN * (i+1))-0.75;
				float y2 = (1.5/divM * (j+1))-0.75;

				float c;

				if (((i + j) % 2) == 0) c = 0.5;
				else c = 0.4;

				points[ind] = vec4(x1, y1, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;
				points[ind] = vec4(x1, y2, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;
				points[ind] = vec4(x2, y2, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;

				points[ind] = vec4(x2, y2, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;
				points[ind] = vec4(x2, y1, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;
				points[ind] = vec4(x1, y1, 0.0, 1.0); colors[ind] = vec4(c,c,c,1.0); ind++;
				
			}
		}

	}

	void initPlain(int ln, int lt) {
		divN = ln;
		divM = lt;
		if (div != 0) div = divM/2;

		numVertices = ln*lt*2*3;

		if (points != NULL) delete [] points;
		if (colors != NULL) delete [] colors;

		points = new vec4[numVertices];
		colors = new vec4[numVertices];

		int sizeofPoints = sizeof(vec4)*numVertices;
		int sizeofColors = sizeof(vec4)*numVertices;
		
		makePlain();
		if(vao == 0)
			glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		if(buffer == 0)
			glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeofPoints + sizeofColors,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			sizeofPoints, points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofPoints,
			sizeofColors, colors);
		
		if (prog == 0) {
			prog =
				InitShader("vshader.glsl", "fshader.glsl");
			glUseProgram(prog);
		}
		
		GLuint vPosition = glGetAttribLocation(prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT,
			false, 0, BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT,
			false, 0, BUFFER_OFFSET(sizeofPoints));
	}

	void draw(float time) {
		glBindVertexArray(vao);
		glUseProgram(prog);

		GLuint uTime = glGetUniformLocation(prog, "uTime");
		glUniform1f(uTime, time);

		GLuint uDiv = glGetUniformLocation(prog, "uDiv");
		glUniform1f(uDiv, div);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
};