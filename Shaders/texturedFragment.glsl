#version 330 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform sampler2D diffuseTex;
//uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 jointWeights;
	//vec4 colour;
	//float height;
} IN;

out vec4 fragColour;
void main(void){
	fragColour = texture(diffuseTex, IN.texCoord);
	if(IN.jointWeights.y == 0){
		//fragColour *= vec4(1,0,0,1);
	}
	//if(IN.height > 180){
	//	fragColour = vec4(1.0,1.0,1.0,1.0) - texture(diffuseTex, IN.texCoord)/5;
	//}
}