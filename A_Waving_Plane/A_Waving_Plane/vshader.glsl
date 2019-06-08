#version 330

uniform float uTime;
uniform float uDiv;
in vec4 vPosition;
in vec4 vColor;
out vec4 color;
out vec4 position;

void main()
{
	color = vColor;
	position = vPosition;

	float theta, s, c, time, div, dist, e;

	time = uTime;
	div = uDiv;

	if(div > 10) div = 12;		//6pi까지 그려준다.

	if(div != 0){
		float pi = 3.141592;
		dist = sqrt((vPosition.x*vPosition.x) + (vPosition.y*vPosition.y)) / (0.75*sqrt(2)/div);	//위치/(R/div) - 2칸당 1pi
		s = sin((dist-time)*pi)/2;

		e = pow(sqrt((vPosition.x*vPosition.x) + (vPosition.y*vPosition.y))/(0.75)-1, 2);
		if(sqrt((vPosition.x*vPosition.x) + (vPosition.y*vPosition.y))/(0.75) > 1) e = 0;
	
		position.z = e*(s + vPosition.z);
	}
	

	mat4 m1 = mat4(1.0f);
	theta = time * 30/180.0*3.141592;
	s = sin(theta);
	c = cos(theta);
	m1[0][0] = c;		m1[1][0] = -s;		m1[2][0] = 0.0f;	m1[3][0] = 0.0f;
	m1[0][1] = s;		m1[1][1] = c;		m1[2][1] = 0.0f;	m1[3][1] = 0.0f;
	m1[0][2] = 0.0f;	m1[1][2] = 0.0f;	m1[2][2] = 1.0f;	m1[3][2] = 0.0f;
	m1[0][3] = 0.0f;	m1[1][3] = 0.0f;	m1[2][3] = 0.0f;	m1[3][3] = 1.0f;

	mat4 m2 = mat4(1.0f);
	theta = 40/180.0*3.141592;
	s = sin(theta);
	c = cos(theta);
	m2[0][0] = 1.0f;	m2[1][0] = 0.0f;	m2[2][0] = 0.0f;	m2[3][0] = 0.0f;
	m2[0][1] = 0.0f;	m2[1][1] = c;		m2[2][1] = -s;		m2[3][1] = 0.0f;
	m2[0][2] = 0.0f;	m2[1][2] = s;		m2[2][2] = c;		m2[3][2] = 0.0f;
	m2[0][3] = 0.0f;	m2[1][3] = 0.0f;	m2[2][3] = 0.0f;	m2[3][3] = 1.0f;

	gl_Position = m2*m1*position;
}