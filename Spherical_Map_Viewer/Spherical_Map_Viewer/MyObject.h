#pragma once

#include <vgl.h>
#include <vec.h>
#include <algorithm>

using namespace std;

struct MyObjectVertex
{
	vec4 position;
	vec4 color;
	vec3 normal;
};

class MyObject
{
public:
	MyObject(void);
	~MyObject(void);

	int NumVertices;

	MyObjectVertex * Vertices;
	//GLuint Init(int la_slice, int lo_slice, vec4 color=vec4(0.5,0.5,0.5,1));
	GLuint Init(const char * filename);
	void SetPositionAndOtherAttributes(GLuint program);

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};



MyObject::MyObject(void)
{
	bInitialized = false;
	NumVertices = 0;
	Vertices = NULL;
}

MyObject::~MyObject(void)
{
	if(Vertices != NULL) 
		delete [] Vertices;
}

GLuint MyObject::Init(const char * filename)
{
	FILE * file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("file not found!");
		return 0;
	}
	int numV = 0;
	int numF = 0;
	char buf[255];
	while(!feof(file)){
		fgets(buf, 255, file);
		if(buf[0] == 'v') numV ++;
		if(buf[0] == 'f') numF ++;
	}
	printf("vertex : %d, face: %d \n", numV, numF);
	fclose(file);


	// The Cube should be initialized only once;
	if(bInitialized == true) return vao;

	NumVertices = numF*3;
	Vertices = new MyObjectVertex [NumVertices];

	vec3 * pos = new vec3[numV];
	int ** face = new int * [numF];
	for(int i=0; i<numF; i++)
		face[i] = new int[3];

	int indV = 0;
	int indF = 0;
	file = fopen(filename, "r");
	while(!feof(file)){
		fgets(buf, 255, file);
		if(buf[0] == 'v')
		{
			float x, y, z;
			char c;
			sscanf(buf, "%c %f %f %f", &c, &x, &y, &z);
			pos[indV] = vec3(x,y,z);
			indV++;
		}
		if(buf[0] == 'f')
		{
			int a, b, c;
			char c1;
			sscanf(buf, "%c %d %d %d", &c1, &a, &b, &c);
			face[indF][0] = a-1;
			face[indF][1] = b-1;
			face[indF][2] = c-1;
			indF++;
		}
	}
	printf("vertex : %d, face: %d \n", indV, indF);
	fclose(file);

	// center != centroid
	vec3 minP = pos[0];
	vec3 maxP = pos[0];
	for(int i=0; i<numV; i++)
	{
		minP.x = min(minP.x, pos[i].x);
		minP.y = min(minP.y, pos[i].y);
		minP.z = min(minP.z, pos[i].z);

		maxP.x = max(maxP.x, pos[i].x);
		maxP.y = max(maxP.y, pos[i].y);
		maxP.z = max(maxP.z, pos[i].z);
	}
	float size = length(maxP - minP);
	vec3 center = (maxP+minP)/2.0f;

	for(int i=0; i<numV; i++)
		pos[i] = (pos[i]-center)/size*2.0f;

	vec3 * fnormal = new vec3[numF];
	for(int i=0; i<numF; i++)
	{
		vec3 a = pos[face[i][0]];
		vec3 b = pos[face[i][1]];
		vec3 c = pos[face[i][2]];
		vec3 p = b-a;
		vec3 q = c-a;
		vec3 n = normalize(cross(p,q));
		fnormal[i] = n;
	}
	vec3 * vnormal = new vec3[numV];
	for(int i=0; i<numV; i++)
		vnormal[i] = vec3(0,0,0);

	for(int j=0; j<numF; j++)
		for(int k=0; k<3; k++)
			vnormal[face[j][k]] += fnormal[j];
	
	for(int i=0; i<numV; i++)
		vnormal[i] = normalize(vnormal[i]);
	

	int cur = 0;
	vec4 color = vec4(1,0.5,0.5,1);
	for(int i=0; i<numF; i++)
	{
		vec3 a = pos[face[i][0]];
		vec3 b = pos[face[i][1]];
		vec3 c = pos[face[i][2]];
			
		vec3 p = b-a;
		vec3 q = c-a;
		vec3 n = normalize(cross(p,q));
		Vertices[cur].position = a;	Vertices[cur].color = color; Vertices[cur].normal = vnormal[face[i][0]]; cur ++;
		Vertices[cur].position = b;	Vertices[cur].color = color; Vertices[cur].normal = vnormal[face[i][1]]; cur ++;
		Vertices[cur].position = c;	Vertices[cur].color = color; Vertices[cur].normal = vnormal[face[i][2]]; cur ++;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyObjectVertex)*NumVertices, Vertices, GL_STATIC_DRAW);
	
	bInitialized = true;
	return vao;
}

void MyObject::SetPositionAndOtherAttributes(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyObjectVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyObjectVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyObjectVertex), BUFFER_OFFSET(sizeof(vec4)+sizeof(vec4)));
}


void MyObject::Draw(GLuint program)
{
	if(!bInitialized) return;			// check whether it is initiazed or not. 
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndOtherAttributes(program);
	
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}