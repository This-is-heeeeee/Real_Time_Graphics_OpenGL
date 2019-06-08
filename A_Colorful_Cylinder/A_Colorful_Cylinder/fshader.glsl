#version 330

in vec4 position;
in vec4 color;
out vec4 fColor;

void main()
{
	fColor = color;
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	
	if(abs(position.z) > 0.45){
		if(position.x * position.x + position.y * position.y < 0.25 * 0.25){
			r = 1.0;
		}
		else if(position.x * position.x + position.y * position.y < 0.4 * 0.4){
			r = g = b = 1.0;
			if(position.z > 0.45) b = 0.0;
		}
		else{
			g = 1.0;
		}
		fColor = vec4(r,g,b,1);
	}

}