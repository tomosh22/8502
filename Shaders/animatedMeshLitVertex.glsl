//#version 400 core
//
//layout(std140) uniform matrices{
//	mat4 modelMatrix;
//	mat4 viewMatrix;
//	mat4 projMatrix;
//	mat4 modelViewMatrix;
//};
//
//in vec3 position;
//in vec2 texCoord;
//in vec4 jointWeights;
// in ivec4 jointIndices;
//
// uniform mat4 joints[128];
//
// out Vertex{
// vec2 texCoord;
// } OUT;
//
// void main(void) {
//	 vec4 localPos = vec4(position, 1.0f);
//	 vec4 skelPos = vec4(0, 0, 0, 0);
//	
//		 for (int i = 0; i < 4; ++i) {
//		 int jointIndex = jointIndices[i];
//		 float jointWeight = jointWeights[i];
//		
//		 skelPos += joints[jointIndex] * localPos * jointWeight;
//		
//	}
//	 mat4 mvp = projMatrix * viewMatrix * modelMatrix;
//	 gl_Position = mvp * vec4(skelPos.xyz, 1.0);
//	 OUT.texCoord = texCoord;
//	
//}

#version 330 core

//uniform mat4 modelMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projMatrix;
uniform int clipHeight;
uniform int clipping;
layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;
in vec4 tangent;
in vec4 jointWeights;
in ivec4 jointIndices;

out Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} OUT;

uniform mat4 joints[128];

void main(void) {
	vec4 plane = vec4(0, -1, 0, clipHeight);
	gl_ClipDistance[0] = clipping * dot(modelMatrix * vec4(position, 1.0), plane);
	/*mat4 modelMatrix = matrices.modelMatrix;
	mat4 viewMatrix = matrices.viewMatrix;
	mat4 projMatrix = matrices.projMatrix;*/
	OUT.texCoord = texCoord;
	OUT.colour = colour;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent, wNormal) * tangent.w;

	vec4 localPos = vec4(position, 1.0f);
	vec4 skelPos = vec4(0, 0, 0, 0);

		for (int i = 0; i < 4; ++i) {
		int jointIndex = jointIndices[i];
		float jointWeight = jointWeights[i];
	
		 skelPos += joints[jointIndex] * localPos * jointWeight;
	
	}
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(skelPos.xyz, 1.0);

}