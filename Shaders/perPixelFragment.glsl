#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec4 

in Vertex {
	vec2 texCoord;
	//vec4 jointWeights;
	vec4 colour;
	//float height;
} IN;

out vec4 fragColour;
void main(void){
	if(useTexture > 0){
		fragColour = texture(diffuseTex, IN.texCoord);
	}
	else{
		fragColour = IN.colour;
	}
	
}