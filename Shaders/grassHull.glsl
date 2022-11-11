#version 410 core
layout (quads,ccw, point_mode) in;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};


out int idOut;

void main() {  
	mat4 mvp = projMatrix*viewMatrix*modelMatrix;
	vec4 pos;
	if(false){
		float u = gl_TessCoord.x;
		float v = gl_TessCoord.y;
		float w = gl_TessCoord.z;

		vec4 pos0 = gl_in[0].gl_Position;
		vec4 pos1 = gl_in[1].gl_Position;
		vec4 pos2 = gl_in[2].gl_Position;

		pos = u * pos0 + v * pos1 + w * pos2;
	}
	else{
		vec4 p0 = mix(gl_in[0].gl_Position,gl_in[2].gl_Position,gl_TessCoord.x);
		vec4 p1 = mix(gl_in[1].gl_Position,gl_in[3].gl_Position,gl_TessCoord.x);
		pos = mix(p0,p1,gl_TessCoord.y);
	}
	gl_Position = pos;

}  