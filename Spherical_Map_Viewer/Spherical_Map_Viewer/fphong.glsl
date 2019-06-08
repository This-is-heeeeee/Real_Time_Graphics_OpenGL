#version 330

in vec3 N3; 
in vec3 L3; 
in vec3 V3; 
in vec3 RN3;
in vec3 RP3;

out vec4 fColor;

uniform mat4 uModelMat; 
uniform mat4 uProjMat; 
uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform vec4 uEPos;
uniform float uShininess; 
uniform sampler2D uTexture;
uniform float uFresnelPower;
uniform sampler2D uTexDiff;

void main()
{
	vec3 N = normalize(N3); 
	vec3 L = normalize(L3); 
	vec3 V = normalize(V3); 
	vec3 H = normalize(V+L); 

    float NL = max(dot(N, L), 0); 
	float VR = pow(max(dot(H, N), 0), uShininess); 
	
	vec3 view_dir = RP3 - uEPos.xyz;
	vec3 normal = normalize(RN3);
	vec3 dir = reflect(view_dir, normal);
	dir = normalize(dir);

	vec2 UV;
	UV.x = 0.25 - (atan(dir.x,dir.z) / (2*3.141592));
	UV.y = dir.y * 0.5 + 0.5;
	
	vec4 ref = texture(uTexture,UV);

	float F = 0.0;
	float ratio = F + (1.0-F)*pow((1.0+dot(normalize(view_dir),RN3)),uFresnelPower);
	if(uFresnelPower > 10) ratio = 0;

	vec4 refDif;
	refDif = texture2D(uTexDiff,UV);
	
	fColor = mix(uAmb + uDif*NL + uSpc*VR , (ref)*0.5f, ratio); 

	fColor.w = 1; 

}
