#version 330 core

//uniform mat4 modelMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projMatrix;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
};

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;
in vec4 tangent;

out Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} OUT;

void main(void){
	/*mat4 modelMatrix = matrices.modelMatrix;
	mat4 viewMatrix = matrices.viewMatrix;
	mat4 projMatrix = matrices.projMatrix;*/
	mat4 mvp = projMatrix*viewMatrix*modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
	OUT.texCoord = texCoord;
	OUT.colour = colour;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent, wNormal) * tangent.w;
}