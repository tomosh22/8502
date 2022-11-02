#version 400 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec4 jointWeights;
in ivec4 jointIndices;

uniform mat4 joints[128];

out Vertex{
	vec2 texCoord;
	vec4 jointWeights;
} OUT;

void main(void){
	vec4 localPos = vec4(position,1.0f);
	vec4 skelPos = vec4(0,0,0,0);
	int jointIndex;
	float jointWeight;
	for(int x = 0; x < 4; x++){
		jointIndex = jointIndices[x];
		jointWeight = jointWeights[x];
		skelPos += joints[jointIndex] * localPos * jointWeight;
	}
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(skelPos.xyz,1.0);
	OUT.texCoord = texCoord;
	OUT.jointWeights = jointWeights;
}