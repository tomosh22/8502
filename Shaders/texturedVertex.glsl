#version 330 core

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};


uniform vec4 nodeColour;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex{
	vec4 clipSpace;
	vec4 colour;
	//float height;
} OUT;

void main(void){
	
	mat4 mvp = projMatrix*viewMatrix*modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
	OUT.clipSpace = gl_Position;
	//OUT.colour = nodeColour;
	//OUT.height = position.y;
}