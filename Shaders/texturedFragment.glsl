#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D reflectTex;
uniform sampler2D refractTex;
uniform int useTexture;
uniform float blendFactor;

in Vertex {
	vec4 clipSpace;
	//vec4 jointWeights;
	vec4 colour;
	//float height;
} IN;

out vec4 fragColour;
void main(void){
	if(true){
		vec2 texCoord = (IN.clipSpace.xy / IN.clipSpace.w) / 2 + 0.5;
		//texCoord -= vec2(0.02, 0.02);
		
		//texCoord = clamp(texCoord,0.001, 0.999);
		//if (texCoord.x < 0) { texCoord.x = 0.0; }
		vec4 reflectColour = texture(reflectTex, vec2(texCoord.x,1-texCoord.y));
		fragColour = mix(reflectColour,texture(refractTex, texCoord),blendFactor);
		fragColour += vec4(-0.1, 0, 0.1, 0);
		//fragColour = vec4(texCoord + vec2(0.5,0.5), 0, 1);
		//fragColour = vec4(1, 0, 0, 1);
	}
	else{
		//fragColour = IN.colour;
	}
	
	//if(IN.jointWeights.y == 0){
		//fragColour *= vec4(1,0,0,1);
	//}
	//if(IN.height > 180){
	//	fragColour = vec4(1.0,1.0,1.0,1.0) - texture(diffuseTex, IN.texCoord)/5;
	//}
	//fragColour = vec4(0,1,0,1);
}