#version 330 core

uniform sampler2D originalTex;
uniform sampler2D shiftedTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColor;
void main(void){
	fragColor = texture(shiftedTex, IN.texCoord) * texture(originalTex, IN.texCoord);
	//fragColor = vec4(1,0,0,1);
}