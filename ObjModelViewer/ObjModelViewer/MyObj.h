#pragma once

#include <vgl.h>
#include <vec.h>

struct MyObjVertex
{
	vec4 position;
	vec4 color;
	vec3 normal;
};

class MyObj {
public:
	MyObj();
	~MyObj();

	int NumVertices;
	vec3 *n;
	float *count;

	MyObjVertex * Vertices;
	GLuint Init(int vertexNum, int faceNum, vec3 *ver, vec3 *face, vec4 color, bool mode);
	void SetPositionAndOtherAttributes(GLuint program);
	void reShading(int vertexNum, int faceNum, vec3 *ver, vec3 *face, bool mode);

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};

MyObj::MyObj() {
	bInitialized = false;
	Vertices = NULL;
}

MyObj::~MyObj()
{
	if (Vertices != NULL)
		delete[] Vertices;
}

GLuint MyObj::Init(int vertexNum, int faceNum, vec3 *ver, vec3 *face, vec4 color, bool mode) {
	if (bInitialized) return vao;

	n = new vec3[vertexNum];
	count = new float[vertexNum];

	for (int i = 0; i < vertexNum; i++) {
		n[i] = vec3(0, 0, 0);
		count[i] = 0;
	}

	NumVertices = faceNum * 3;
	Vertices = new MyObjVertex[NumVertices];
	
	int cur = 0;

	for (int i = 0; i < faceNum; i++) {
		int a = (int)face[i].x;
		int b = (int)face[i].y;
		int c = (int)face[i].z;
		vec4 p = ver[b] - ver[a];
		vec4 q = ver[c] - ver[a];
		vec3 p3 = vec3(p.x, p.y, p.z);
		vec3 q3 = vec3(q.x, q.y, q.z);
		
		vec3 normal = normalize(cross(p3, q3));

		Vertices[cur].position = ver[a];	Vertices[cur].color = color;	cur++;
		Vertices[cur].position = ver[b];	Vertices[cur].color = color;	cur++;
		Vertices[cur].position = ver[c];	Vertices[cur].color = color;	cur++;

		if (mode) {
			count[a]++;
			count[b]++;
			count[c]++;

			n[a] += normal;
			n[b] += normal;
			n[c] += normal;
		}
		else {
			Vertices[cur].normal = normal;
			Vertices[cur].normal = normal;
			Vertices[cur].normal = normal;
		}
	}
	if (mode) {
		cur = 0;
		for (int i = 0; i < faceNum; i++) {
			int a = (int)face[i].x;
			int b = (int)face[i].y;
			int c = (int)face[i].z;
			Vertices[cur].normal = n[a] / count[a]; cur++;
			Vertices[cur].normal = n[b] / count[b]; cur++;
			Vertices[cur].normal = n[c] / count[c]; cur++;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyObjVertex)*NumVertices, Vertices, GL_STATIC_DRAW);

	bInitialized = true;
	return vao;
}

void MyObj::SetPositionAndOtherAttributes(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(vec4) + sizeof(vec4)));
}

void MyObj::reShading(int vertexNum, int faceNum, vec3 *ver, vec3 *face, bool mode) {
	for (int i = 0; i < vertexNum; i++) {
		n[i] = vec3(0, 0, 0);
		count[i] = 0;
	}
	int cur = 0;

	for (int i = 0; i < faceNum; i++) {
		int a = (int)face[i].x;
		int b = (int)face[i].y;
		int c = (int)face[i].z;
		vec4 p = ver[b] - ver[a];
		vec4 q = ver[c] - ver[a];
		vec3 p3 = vec3(p.x, p.y, p.z);
		vec3 q3 = vec3(q.x, q.y, q.z);

		vec3 normal = normalize(cross(p3, q3));


		if (mode) {
			count[a]++;
			count[b]++;
			count[c]++;

			n[a] += normal;	cur++;
			n[b] += normal;	cur++;
			n[c] += normal;	cur++;
		}
		else {
			Vertices[cur].normal = normal;	cur++;
			Vertices[cur].normal = normal;	cur++;
			Vertices[cur].normal = normal;	cur++;
		}
	}
	if (mode) {
		cur = 0;
		for (int i = 0; i < faceNum; i++) {
			int a = (int)face[i].x;
			int b = (int)face[i].y;
			int c = (int)face[i].z;
			Vertices[cur].normal = n[a] / count[a]; cur++;
			Vertices[cur].normal = n[b] / count[b]; cur++;
			Vertices[cur].normal = n[c] / count[c]; cur++;
		}
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyObjVertex)*NumVertices, Vertices, GL_STATIC_DRAW);
}

void MyObj::Draw(GLuint program)
{
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndOtherAttributes(program);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}