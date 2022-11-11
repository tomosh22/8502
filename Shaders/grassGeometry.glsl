#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

layout(std140) uniform matrices{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 modelViewMatrix;
};
uniform float time;
out Vertex{
	vec4 colour;
} OUT;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {  
    /*mat4 mv;
    mv[3][0] = modelMatrix[3][0];
    mv[3][1] = modelMatrix[3][1];
    mv[3][2] = modelMatrix[3][2];

    mv[0][0] = viewMatrix[0][0];
    mv[1][1] = viewMatrix[1][1];
    mv[2][2] = viewMatrix[2][2];
    mv[3][3] = viewMatrix[3][3];

    mv[0][1] = viewMatrix[1][0];
    mv[1][2] = viewMatrix[2][1];
    mv[2][0] = viewMatrix[0][2];

    mv[0][2] = viewMatrix[2][0];
    mv[1][0] = viewMatrix[0][1];
    mv[2][1] = viewMatrix[1][2];*/

    mat4 mv = mat4(
        viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], 0,
        viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0,
        viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], 0,
        modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2], 1
    );

    
    vec4 darkGreen = vec4(0, 0.2, 0, 1);

    mat4 mvp = projMatrix*viewMatrix*modelMatrix;
    //mat4 mvp = projMatrix*viewMatrix*mv;
    
    gl_Position = mvp * (gl_in[0].gl_Position + vec4(sin(time)/10,0,-0.5,0));
    OUT.colour = vec4(0, 1, 0, 1);
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(-0.025,-0.025,0,0));
    OUT.colour = darkGreen;
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(-0.025, 0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    EndPrimitive();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(sin(time) / 10, 0, -0.5, 0));
    OUT.colour = vec4(0, 1, 0, 1);
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(0.025, -0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(0.025, 0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    EndPrimitive();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(sin(time) / 10, 0, -0.5, 0));
    OUT.colour = vec4(0, 1, 0, 1);
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(-0.025, -0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(0.025, -0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    EndPrimitive();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(sin(time) / 10, 0, -0.5, 0));
    OUT.colour = vec4(0, 1, 0, 1);
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(-0.025, 0.025, 0, 0));
    OUT.colour = darkGreen;
    EmitVertex();

    gl_Position = mvp * (gl_in[0].gl_Position + vec4(0.025, 0.025, 0, 0));
    OUT.colour = darkGreen;
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