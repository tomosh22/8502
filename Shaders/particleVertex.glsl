#version 330 core

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec4 column0;
in vec4 column1;
in vec4 column2;
in vec4 column3;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	//mat4 modelViewMatrix;
};

out Vertex{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void){
	OUT.texCoord.x = texCoord.x;
	OUT.texCoord.y = 1-texCoord.y;
	OUT.colour = colour;
	mat4 modelViewMatrix = mat4(column0,column1,column2,column3);
	mat4 mvp = projMatrix * modelViewMatrix;
	gl_Position =  mvp * vec4(position,1.0);
}