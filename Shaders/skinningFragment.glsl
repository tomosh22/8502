#version 330 core
uniform sampler2D diffuseTex;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
	vec4 fogColour;
	bool drawNormals;
};

in Vertex {
vec2 texCoord;


	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
	float fogFactor;

} IN;

out vec4 fragColour[2];
void main(void) {
 fragColour[0] = texture(diffuseTex , IN.texCoord );


	fragColour[0] = texture2D(diffuseTex,IN.texCoord);
	
	fragColour[1] = vec4(IN.normal.xyz*0.5 + 0.5,1);
	if(drawNormals){
		fragColour[0] = vec4(IN.normal.xyz*0.5 + 0.5,1);
	}
 }