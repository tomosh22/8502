#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};

out Vertex{
	vec4 colour;
} OUT;



void main() {  

    mat4 mvp = projMatrix*viewMatrix*modelMatrix;

    gl_Position = mvp * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(0,10,0,0));
    EmitVertex();

    EndPrimitive();
    return;










    //mat4 mvp = projMatrix*viewMatrix*modelMatrix;
    gl_Position = mvp * gl_in[0].gl_Position;
    OUT.colour = vec4(1,0,0,1);
    EmitVertex();

    gl_Position = mvp * gl_in[1].gl_Position;
    OUT.colour = vec4(1,0,0,1);
    EmitVertex();

    gl_Position = mvp * gl_in[2].gl_Position;
    OUT.colour = vec4(1,0,0,1);
    EmitVertex();

    EndPrimitive();

    gl_Position = mvp * gl_in[0].gl_Position + vec4(0,50,0,0);
    OUT.colour = vec4(0,1,0,1);
    EmitVertex();

    gl_Position = mvp * gl_in[1].gl_Position + vec4(0,50,0,0);
    OUT.colour = vec4(0,1,0,1);
    EmitVertex();

    gl_Position = mvp * gl_in[2].gl_Position + vec4(0,50,0,0);
    OUT.colour = vec4(0,1,0,1);
    EmitVertex();

    EndPrimitive();

    gl_Position = mvp * gl_in[0].gl_Position + vec4(0,100,0,0);
    OUT.colour = vec4(0,0,1,1);
    EmitVertex();

    gl_Position = mvp * gl_in[1].gl_Position + vec4(0,100,0,0);
    OUT.colour = vec4(0,0,1,1);
    EmitVertex();

    gl_Position = mvp * gl_in[2].gl_Position + vec4(0,100,0,0);
    OUT.colour = vec4(0,0,1,1);
    EmitVertex();

    EndPrimitive();

}  