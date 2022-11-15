#version 330 core
layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};
in vec3 position;
out Vertex{
	vec4 colour;
} OUT;

void main(void){
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position,1.0);
}