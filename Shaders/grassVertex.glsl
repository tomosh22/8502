#version 330 core

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};

in vec3 position;



void main(void){
	mat4 mvp = projMatrix*viewMatrix*modelMatrix;
	gl_Position = vec4(position, 1.0);
}