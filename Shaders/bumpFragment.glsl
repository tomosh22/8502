#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
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
	vec3 tangent;
	vec3 binormal;
	float fogFactor;
} IN;

out vec4 fragColour[2];
void main(void){
	
	

	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 incident = normalize(vec3(0,1,0));
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 bumpNormal = texture(bumpTex, IN.texCoord).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2 - 1));
	float lambert = max(dot(incident, bumpNormal),0);
	float specFactor = clamp(dot(halfDir,bumpNormal),0,1);
	specFactor = pow(specFactor,30);
	vec3 surface = (diffuse.rgb * vec3(1,1,1));
	fragColour[0].rgb = surface * lambert;
	fragColour[0].rgb += (vec3(1,1,1) * specFactor) * 0.33;
	fragColour[0].rgb += surface * 0.1;
	fragColour[0].a = diffuse.a;
	fragColour[0] += diffuse * 0.5;
	//fragColour[0] *= 100;

	
	vec3 normal = texture2D(bumpTex,IN.texCoord).rgb * 2 - 1;
	normal = normalize(TBN * normalize(normal));
	//fragColour[0] = texture2D(diffuseTex,IN.texCoord);
	fragColour[1] = vec4(normal.xyz*0.5 + 0.5,1);


	//fragColour = vec4(1, 0, 0, 1);

	//fragColour = mix(vec4(0.4, 0.2, 0.2,1), fragColour, IN.fogFactor);
	//fragColour = vec4(IN.fogFactor, IN.fogFactor, IN.fogFactor,1);

}