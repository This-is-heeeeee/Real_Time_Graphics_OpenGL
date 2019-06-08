#version 330

in vec4 color;
in vec2 texCoord;
in vec4 fPosition;

out vec4 fColor;

uniform sampler2D uTexture;

void main()
{
	vec4 tex = texture2D(uTexture, texCoord);

	if(fPosition.z > 0) discard;
	fColor =tex;
	fColor.w = 1; 

}
