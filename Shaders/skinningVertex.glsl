#version 400 core

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};

in vec3 position;
in vec2 texCoord;
in vec4 jointWeights;
in ivec4 jointIndices;
in vec3 normal;
in vec4 tangent;

uniform mat4 joints[128];

out Vertex{
	vec2 texCoord;
	//vec4 jointWeights;

	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} OUT;

void main(void){
	vec4 localPos = vec4(position,1.0f);
	vec4 skelPos = vec4(0,0,0,0);
	int jointIndex;
	float jointWeight;
	OUT.normal = vec3(0,0,0);
	for(int x = 0; x < 4; x++){
		jointIndex = jointIndices[x];
		jointWeight = jointWeights[x];
		skelPos += joints[jointIndex] * localPos * jointWeight;
		OUT.normal += mat3(joints[jointIndex]) * normal * jointWeight;
	}
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(skelPos.xyz,1.0);
	OUT.texCoord = texCoord;
	//OUT.jointWeights = jointWeights;


}