#version 400 core
layout (points) in;
layout (line_strip, max_vertices = 100) out;

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

//found this on stackoverflow, absolutely no idea why it works
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
    
    vec4 base = modelMatrix * gl_in[0].gl_Position;
    for (int x = 0; x < 10; x++)
    {
        float freq = time + rand(base.xz + 7) / 2;
        float amp = rand(base.xz + 8)/20 + 0.2;
        float height = -0.5 + rand(base.xz + 9)/5;
        //vec4 offset = vec4(sin(freq))
        gl_Position = mvp * (gl_in[0].gl_Position + vec4(sin(freq) * amp, 0, height, 0));
        OUT.colour = vec4(rand(base.xz+1), 1 - rand(base.xz + 2) / 10, 0, 1);
        EmitVertex();

        gl_Position = mvp * (gl_in[0].gl_Position + vec4(rand(base.xz+3)/20 + rand(base.xz + 4) / 5, rand(base.xz+5)/20 + rand(base.xz + 6) / 5, 0, 0));
        OUT.colour = darkGreen;
        EmitVertex();
        base += 6;

        EndPrimitive();

    }

}  