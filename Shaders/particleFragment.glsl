#version 330 core
uniform sampler2D particleTex;
in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void){
	vec4 sample = texture(particleTex, IN.texCoord); 
	if(sample.a < 0.2){discard;}
	fragColour = max(sample.r,(max(sample.g,sample.b))) * IN.colour * sample.a;
}