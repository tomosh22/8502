#version 330 core

uniform samplerCube cubeTex;
in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour;
void main(void){



	fragColour = texture(cubeTex, normalize(IN.viewDir));

	return;
	//might come back to this if i implement fog
	vec4 fogColour = vec4(0.4, 0.2, 0.2, 1);
	float blendFactor = clamp((normalize(IN.viewDir).y - 0) / 1, 0.0, 1.0);
	if (IN.viewDir.y > 0) {
		fragColour = texture(cubeTex, normalize(IN.viewDir));
	}
	else {
		if (IN.viewDir.y > -0.3) {
			fragColour = mix(texture(cubeTex, normalize(IN.viewDir)), fogColour, IN.viewDir.y / -0.3);
		}
		else {
			fragColour = fogColour;
		}
		
	}
}