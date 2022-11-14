#version 330 core

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};


uniform vec4 nodeColour;

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;
in vec4 tangent;

out Vertex{
	vec4 clipSpace;
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
	//float height;
} OUT;

void main(void){
	OUT.texCoord = texCoord;
	mat4 mvp = projMatrix*viewMatrix*modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
	OUT.clipSpace = gl_Position;
	//OUT.colour = nodeColour;
	//OUT.height = position.y;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent, wNormal) * tangent.w;
}