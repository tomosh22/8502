#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

layout(location = 1) in vec4 column0;
layout(location = 3) in vec4 column1;
layout(location = 4) in vec4 column2;
layout(location = 5) in vec4 column3;

layout(location = 6) in vec4 colour;


layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};



out Vertex{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void){
	OUT.texCoord.x = texCoord.x;
	OUT.texCoord.y = 1-texCoord.y;
	OUT.colour = colour;

	mat4 mvMatrix = mat4(column0, column1, column2, column3);
	mat4 mvp = projMatrix * mvMatrix;


	gl_Position =  mvp * vec4(position,1.0);
}