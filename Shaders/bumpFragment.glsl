#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform vec3 cameraPos;
uniform vec4 diffuseColour;
uniform vec4 specularColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform samplerCube shadowTex;
uniform bool renderFog;

in Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
	float fogFactor;
	vec4 shadowProj;
	vec3 worldDir;
} IN;

out vec4 fragColour;
void main(void){
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 bumpNormal = texture(bumpTex, IN.texCoord).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2 - 1));
	float lambert = max(dot(incident, bumpNormal),0);
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1 - clamp(distance/lightRadius,0,1);
	float specFactor = clamp(dot(halfDir,bumpNormal),0,1);
	specFactor = pow(specFactor,30);

	float shadow = 1.0;
	vec3 shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;
	if((abs(shadowNDC.x)<1 &&
		abs(shadowNDC.y) < 1 &&
		abs(shadowNDC.z) < 1) || true
	){
		vec3 biasCoord = shadowNDC * 0.5 + 0.5;
		float shadowZ = texture(shadowTex,vec3(worldDir)).x;
		if(shadowZ < biasCoord.z){shadow = 0;}
	}

	/*vec3 fragToLight = IN.worldPos - lightPos;
	float closestDepth = texture(shadowTex, fragToLight).r;
	closestDepth *= 2000;
	float currentDepth = length(fragToLight);
	float shadow = currentDepth > closestDepth ? 1 : 0;*/


	vec3 surface = (diffuse.rgb * diffuseColour.rgb);
	fragColour.rgb = surface * lambert * attenuation;
	fragColour.rgb += (specularColour.rgb * specFactor)*attenuation * 0.33;
	fragColour.rgb *= shadow;
	fragColour.rgb += surface * 0.1;
	fragColour.a = diffuse.a;
	fragColour += diffuse * 0.5;
	//fragColour = vec4(1, 0, 0, 1);

	if(renderFog){
		fragColour = mix(vec4(0.4, 0.2, 0.2,1), fragColour, IN.fogFactor);
	}
	//fragColour = vec4(IN.fogFactor, IN.fogFactor, IN.fogFactor,1);
	//fragColour = vec4(1,0,0,1);

}