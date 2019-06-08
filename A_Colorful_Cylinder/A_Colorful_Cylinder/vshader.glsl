#version 330


uniform float uTime;
in vec4 vPosition;
in vec4 vColor;
out vec4 position;
out vec4 color;

void main()
{
	position = vPosition;
	color = vColor;

	float theta, s, c, time;

	time = uTime;
	mat4 m1 = mat4(1.0f);
	theta = time*30/180.0*3.141592;
	s = sin(theta);
	c = cos(theta);
	m1[0][0] = c;		m1[1][0] = -s;		m1[2][0] = 0.0f;	m1[3][0] = 0.0f;
	m1[0][1] = s;		m1[1][1] = c;		m1[2][1] = 0.0f;	m1[3][1] = 0.0f;
	m1[0][2] = 0.0f;	m1[1][2] = 0.0f;	m1[2][2] = 1.0f;	m1[3][2] = 0.0f;
	m1[0][3] = 0.0f;	m1[1][3] = 0.0f;	m1[2][3] = 0.0f;	m1[3][3] = 1.0f;

	mat4 m2 = mat4(1.0f);
	theta = time*40/180.0*3.141592;
	s = sin(theta);
	c = cos(theta);
	m2[0][0] = c;		m2[1][0] = 0.0f;	m2[2][0] = -s;		m2[3][0] = 0.0f;
	m2[0][1] = 0.0f;	m2[1][1] = 1.0f;	m2[2][1] = 0.0f;	m2[3][1] = 0.0f;
	m2[0][2] = s;		m2[1][2] = 0.0f;	m2[2][2] = c;		m2[3][2] = 0.0f;
	m2[0][3] = 0.0f;	m2[1][3] = 0.0f;	m2[2][3] = 0.0f;	m2[3][3] = 1.0f;

	mat4 m3 = mat4(1.0f);
	theta = time*50/180.0*3.141592;
	s = sin(theta);
	c = cos(theta);
	m3[0][0] = 1.0f;	m3[1][0] = 0.0f;	m3[2][0] = 0.0f;	m3[3][0] = 0.0f;
	m3[0][1] = 0.0f;	m3[1][1] = c;		m3[2][1] = -s;		m3[3][1] = 0.0f;
	m3[0][2] = 0.0f;	m3[1][2] = s;		m3[2][2] = c;		m3[3][2] = 0.0f;
	m3[0][3] = 0.0f;	m3[1][3] = 0.0f;	m3[2][3] = 0.0f;	m3[3][3] = 1.0f;

	gl_Position = m3*m2*m1*vPosition;
}