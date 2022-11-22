#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D diffuseTex0;
uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform sampler2D bumpTex0;
uniform sampler2D bumpTex1;
uniform sampler2D bumpTex2;
uniform sampler2D splatMap;
uniform vec3 cameraPos;
uniform vec4 diffuseColour;
uniform vec4 specularColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform vec3 lightDir;


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
	
	
	
	vec3 splat = normalize(texture(splatMap, IN.texCoord/32).rgb);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec3 incident = normalize(lightDir);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	vec4 diffuse = texture(diffuseTex0, IN.texCoord) * splat.r;
	diffuse += texture(diffuseTex1, IN.texCoord) * splat.g;
	diffuse += texture(diffuseTex2, IN.texCoord) * splat.b;

	vec3 bumpNormal = texture(bumpTex0, IN.texCoord).rgb * splat.r;
	bumpNormal += texture(bumpTex1, IN.texCoord).rgb * splat.g;
	bumpNormal += texture(bumpTex2, IN.texCoord).rgb * splat.b;

	bumpNormal = normalize(TBN * normalize(bumpNormal * 2 - 1));
	float lambert = max(dot(incident, bumpNormal),0);
	float specFactor = clamp(dot(halfDir,bumpNormal),0,1);
	specFactor = pow(specFactor,30);
	vec3 surface = (diffuse.rgb * diffuseColour.rgb);
	fragColour[0].rgb = surface * lambert;
	fragColour[0].rgb += (specularColour.rgb * specFactor) * 0.33;
	fragColour[0].rgb += surface * 0.1;
	fragColour[0].a = diffuse.a;
	fragColour[0] += diffuse * 0.5;
	//fragColour[0] *= 100;

	
	vec3 normal = bumpNormal;
	normal = normalize(TBN * normalize(normal));
	//fragColour[0] = texture2D(diffuseTex,IN.texCoord);
	fragColour[1] = vec4(bumpNormal.xyz*0.5 + 0.5,1);


	//fragColour = vec4(1, 0, 0, 1);

	fragColour[0] = mix(vec4(0.4, 0.2, 0.2,1), fragColour[0], IN.fogFactor);
	//fragColour = vec4(IN.fogFactor, IN.fogFactor, IN.fogFactor,1);

}