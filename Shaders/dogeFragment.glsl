#version 330 core
#extension GL_ARB_bindless_texture : enable


layout(bindless_sampler) uniform sampler2D circuitTex;
layout(bindless_sampler) uniform sampler2D metalTex;
layout(bindless_sampler) uniform sampler2D dogeTex;
uniform float time;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;
void main(void){
	float distance = length(IN.texCoord-vec2(0.5f,0.5f))+time/3.3f;
	fragColour = texture(metalTex, IN.texCoord);
	if(texture(circuitTex,IN.texCoord).x>0.2f){
	//discard;
	if(distance > 0.4f && distance < 0.5f){
		//fragColour = mix(texture(dogeTex, IN.texCoord),texture(metalTex, IN.texCoord),(distance-0.45f)*10);
		fragColour = texture(dogeTex, IN.texCoord);
	}
	else{
		if(distance > 0.1f && distance < 0.2f){
			//fragColour = mix(texture(dogeTex, IN.texCoord),texture(metalTex, IN.texCoord),(distance-0.15f)*10);
			fragColour = texture(dogeTex, IN.texCoord);
		}
		else{
			if(distance > 0.7f && distance < 0.8f){
				//fragColour = mix(texture(dogeTex, IN.texCoord),texture(metalTex, IN.texCoord),(distance-0.75f)*10);
				fragColour = texture(dogeTex, IN.texCoord);
			}
			else{
				//fragColour = fragColour;
			}
		}
	}
	}
	else{}
	

	
}