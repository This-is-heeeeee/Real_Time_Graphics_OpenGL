#version 330

uniform float uTime;
in vec4 color;
in vec4 position;
out vec4 fColor;

void main()
{
	fColor = color;
	float r, g, b;
	r = color.r;
	g = color.g;
	b = color.b;
	if(position.z > 0.0){
		b += position.z/0.5;
	}
	else if(position.z < 0.0){
		r += (-position.z)/0.5;
	}
	fColor = vec4(r,g,b,1.0);
	
}