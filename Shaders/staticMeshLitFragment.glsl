#version 330 core

uniform sampler2D diffuseTex;
uniform int useTexture;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void){
	fragColour = texture(diffuseTex, IN.texCoord);
	fragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}