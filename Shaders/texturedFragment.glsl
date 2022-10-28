#version 330 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform sampler2D diffuseTex;
//uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;
void main(void){
	fragColour = texture(diffuseTex, IN.texCoord);
}