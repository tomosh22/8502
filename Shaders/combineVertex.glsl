#version 330 core

in vec3 position;
in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

//uniform mat4 viewMatrix;
//uniform mat4 projMatrix;

out Vertex {
vec2 texCoord;

vec3 viewDir;
} OUT;

 void main(void) {
 gl_Position = vec4(position , 1.0);
 OUT.texCoord = texCoord;

 vec3 pos = position;
 mat4 invproj = inverse(projMatrix);
 pos.xy *= vec2(invproj[0][0], invproj[1][1]);
 pos.z = -1;
 OUT.viewDir = transpose(mat3(viewMatrix)) * normalize(pos);
 }