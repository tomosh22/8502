#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 diffuseColour;
uniform vec4 specularColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;
void main(void){
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	float lambert = max(dot(incident, IN.normal),0);
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1 - clamp(distance/lightRadius,0,1);
	float specFactor = clamp(dot(halfDir,IN.normal),0,1);
	specFactor = pow(specFactor,60);
	vec3 surface = (diffuse.rgb * diffuseColour.rgb);
	fragColour.rgb = surface * lambert * attenuation;
	fragColour.rgb += (specularColour.rgb * specFactor)*attenuation * 0.33;
	fragColour.rgb += surface * 0.1;
	fragColour.a = diffuse.a;
	
}