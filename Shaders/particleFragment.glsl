#version 330 core

uniform sampler2D particleTex;
in Vertex{
	vec2 texCoord;
} IN;

out vec4 fragColour;
void main(void){
	fragColour = texture(particleTex, IN.texCoord);
	//fragColour = vec4(1, 0, 0, 1);
}