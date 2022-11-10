#version 410 core

layout (vertices=4) out;


uniform int tesselationLevel;

void main(){
	gl_TessLevelOuter[0] = tesselationLevel;
	gl_TessLevelOuter[1] = tesselationLevel;
	gl_TessLevelOuter[2] = tesselationLevel;
	gl_TessLevelOuter[3] = tesselationLevel;

	gl_TessLevelInner[0] = tesselationLevel;
	gl_TessLevelInner[1] = tesselationLevel;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
}