#version 330 core

uniform sampler2D sceneTex;
uniform int isVertical;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColor;
const float scaleFactors[7] = float[](.006 , 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);
void main(void){
	fragColor = vec4(0,0,0,1);
	vec2 delta;
	if(isVertical == 1){
		delta = dFdy(IN.texCoord);
	}
	else{
		delta = dFdx(IN.texCoord);
	}
	vec2 offset;
	vec4 temp;
	for(int x = 0; x < 7; x++){
		offset = delta * (x-3);
		temp = texture2D(sceneTex,IN.texCoord.xy+offset);
		fragColor += temp * scaleFactors[x];
	}
	//fragColor = vec4(1,0,0,1);
}