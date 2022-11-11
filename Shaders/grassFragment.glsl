#version 330 core
#extension GL_ARB_bindless_texture : enable


in Vertex{
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void){
	fragColour = IN.colour;
}