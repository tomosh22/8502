#version 330 core
uniform sampler2D particleTex;
in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour[2];
void main(void){
	vec4 sample = texture(particleTex, IN.texCoord); 
	if(sample.a < 0.6){discard;}
	fragColour[0] = max(sample.r,(max(sample.g,sample.b))) * IN.colour * sample.a;
	fragColour[1] = vec4(0,1,0, 1);
}