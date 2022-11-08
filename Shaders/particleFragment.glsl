#version 330 core

in vec4 colourInstanced;

uniform sampler2D particleTex;
uniform vec4 colour;
in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void){
	vec4 sample = texture(particleTex, IN.texCoord); 
	fragColour = max(sample.r,(max(sample.g,sample.b))) * colour * sample.a;
	fragColour = colourInstanced;
}