#version 330 core

in vec3 position;
in vec4 colour;

out Vertex{
	vec2 texCoord;
} OUT;

void main(void){
	gl_Position = vec4(position,1.0);
}