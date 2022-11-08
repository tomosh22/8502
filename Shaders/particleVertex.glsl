#version 330 core

in vec3 position;
in vec2 texCoord;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};

out Vertex{
	vec2 texCoord;
} OUT;

void main(void){
	OUT.texCoord = texCoord;
	mat4 mvp = projMatrix * modelViewMatrix;
	gl_Position =  mvp * vec4(position,1.0);
}