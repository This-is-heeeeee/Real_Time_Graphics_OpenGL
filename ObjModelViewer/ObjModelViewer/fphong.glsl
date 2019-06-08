#version 330

in  vec4 color;
in  vec3 N3;
in  vec3 L3;
in  vec3 R3;
in  vec3 V3;

uniform float uGloss;
uniform float uKs;

out vec4 fColor;

void
main()
{
	vec3 N = normalize(N3);
	vec3 L = normalize(L3);
	vec3 R = normalize(R3);
	vec3 V = normalize(V3);

	vec4 Il = vec4(1,1,1,1);
	vec4 ka = vec4(0.2,0.2,0.2,1);
	vec4 kd = color;
	vec4 ks = vec4(uKs,uKs,uKs,1);

	float dif = max(dot(N,L),0);
	
	float gloss = uGloss;
	float spec = pow(max(dot(V, R),0), gloss);
	
	fColor = ka*Il + kd*Il*dif + ks*Il*spec;

}