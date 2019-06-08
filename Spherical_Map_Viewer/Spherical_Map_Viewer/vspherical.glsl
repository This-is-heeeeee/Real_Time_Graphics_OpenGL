#version 330

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal;
in  vec2 vTexCoord;

out vec4 color; 
out vec2 texCoord;
out vec4 fPosition;

uniform mat4 uCamera;
uniform mat4 uMat;

void main()
{
	gl_Position  = uMat*vPosition;
	gl_Position *= vec4(1,1,-1,1);

	color = vec4(vTexCoord.s, 0, vTexCoord.t, 1);
	texCoord = vTexCoord;
	texCoord.s = 1.0 - vTexCoord.s;
	fPosition = vPosition*uCamera;
}
