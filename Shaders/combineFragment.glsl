#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseLight;
uniform sampler2D specularLight;

uniform sampler2D depthTex;
uniform samplerCube cubeTex;

in Vertex {
vec2 texCoord;

vec3 viewDir;
} IN;

 out vec4 fragColour;

 void main(void) {
 if (texture(depthTex, IN.texCoord).x == 1) {
		 fragColour = texture(cubeTex, normalize(IN.viewDir));
		 return;
	 }
 vec3 diffuse = texture(diffuseTex , IN.texCoord ).xyz;
 vec3 light = texture(diffuseLight , IN.texCoord ).xyz;
 vec3 specular = texture(specularLight , IN.texCoord ).xyz;

 fragColour.xyz = diffuse * 0.1; // ambient
 fragColour.xyz += diffuse * light; // lambert
 fragColour.xyz += specular; // Specular
 fragColour.a = 1.0;
	 
	 //fragColour = vec4(diffuse,1);
 }