#version 330 core

uniform samplerCube cubeTex;
in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour[2];
void main(void){



	fragColour[0] = texture(cubeTex, normalize(IN.viewDir));
	fragColour[0] *= 1;
	fragColour[1] = vec4(-1, -1, -1, -1);
	return;
	//might come back to this if i implement fog
	vec4 fogColour = vec4(0.4, 0.2, 0.2, 1);
	float blendFactor = clamp((normalize(IN.viewDir).y - 0) / 1, 0.0, 1.0);
	if (IN.viewDir.y > 0) {
		fragColour[0] = texture(cubeTex, normalize(IN.viewDir));
	}
	else {
		if (IN.viewDir.y > -0.3) {
			fragColour[0] = mix(texture(cubeTex, normalize(IN.viewDir)), fogColour, IN.viewDir.y / -0.3);
		}
		else {
			fragColour[0] = fogColour;
		}
		
	}
}